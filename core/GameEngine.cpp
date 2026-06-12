#include "GameEngine.h"
#include <QRandomGenerator>
#include "../scenarios/FallGelbScenario.h"
#include "../scenarios/BritainScenario.h"
#include "../scenarios/WolfPackScenario.h"
#include "../scenarios/StalingradScenario.h"
#include "../scenarios/BerlinScenario.h"

GameEngine::GameEngine(QObject *parent) : QObject(parent) {
    registerScenarios();
}

void GameEngine::registerScenarios() {
    auto addScenario = [this](std::unique_ptr<ScenarioBase> s) {
        ScenarioId id = s->scenarioId();
        s->initialize();
        m_scenarios.emplace(id, std::move(s));
    };
    addScenario(std::make_unique<FallGelbScenario>());
    addScenario(std::make_unique<BritainScenario>());
    addScenario(std::make_unique<WolfPackScenario>());
    addScenario(std::make_unique<StalingradScenario>());
    addScenario(std::make_unique<BerlinScenario>());
}

void GameEngine::newGame(const QString &playerName, PlayerClass cls) {
    m_player = Player(playerName, cls);
    m_player.unlockedScenarios.insert(static_cast<int>(ScenarioId::FallGelb));
    // 根据职业解锁对应的首个场景
    if (cls == PlayerClass::FighterPilot || cls == PlayerClass::BomberPilot)
        m_player.unlockedScenarios.insert(static_cast<int>(ScenarioId::Britain));
    if (cls == PlayerClass::SubmarineCrew || cls == PlayerClass::BattleshipCrew)
        m_player.unlockedScenarios.insert(static_cast<int>(ScenarioId::WolfPack));
}

void GameEngine::loadGame(const Player &savedPlayer) {
    m_player = savedPlayer;
    auto it = m_scenarios.find(m_player.currentScenario);
    if (it != m_scenarios.end()) {
        m_currentScenario = it->second.get();
        // 直接触发导航到当前节点，以通知 UI 刷新，不重置状态
        navigateTo(m_player.currentNodeId);
    }
}

bool GameEngine::startScenario(ScenarioId id) {
    auto it = m_scenarios.find(id);
    if (it == m_scenarios.end()) return false;

    m_currentScenario = it->second.get();
    m_player.currentScenario = id;
    m_player.resetStats();

    QString startId = m_currentScenario->startNodeId(m_player.playerClass);
    navigateTo(startId);
    return true;
}

const StoryNode* GameEngine::currentNode() const {
    if (!m_currentScenario) return nullptr;
    return m_currentScenario->getNode(m_player.currentNodeId);
}

bool GameEngine::makeChoice(int choiceIndex) {
    const StoryNode *node = currentNode();
    if (!node) return false;

    // Narrative 节点只有【继续】按钮，直接跳转
    if (node->type == NodeType::Narrative) {
        navigateTo(node->nextNodeId);
        return true;
    }

    if (choiceIndex < 0 || choiceIndex >= node->choices.size()) return false;

    const Choice &choice = node->choices[choiceIndex];

    // 职业限制检查
    if (choice.classRestricted &&
        !choice.allowedClasses.contains(m_player.playerClass)) return false;

    // 标志条件检查
    for (const QString &flag : choice.requiredFlags) {
        if (!m_player.hasFlag(flag)) return false;
    }

    if (choice.isCombat) {
        int roll = rollDice(choice.bonusClasses);
        bool success = (roll >= choice.combatThreshold);

        if (success) {
            applyChoice(choice, true);
            emit combatResult(true, choice.hpDelta, choice.moraleDelta);
            navigateTo(choice.successNodeId);
        } else {
            m_player.applyHpDelta(choice.failHpDelta);
            m_player.applyMoraleDelta(choice.failMoraleDelta);
            // 战斗失败时也应用标志
            for (const QString &flag : choice.grantedFlags) {
                m_player.setFlag(flag);
                emit flagSet(flag);
            }
            emit combatResult(false, choice.failHpDelta, choice.failMoraleDelta);
            emit statsChanged(m_player.hp, m_player.morale);

            if (m_player.isDead()) {
                QString defeatId = m_currentScenario ? m_currentScenario->defeatNodeId() : QString();
                if (defeatId.isEmpty())
                    defeatId = QStringLiteral("__defeat__");
                navigateTo(defeatId);
                return true;
            }
            navigateTo(choice.failureNodeId);
        }
    } else {
        applyChoice(choice, true);
        navigateTo(choice.nextNodeId);
    }
    return true;
}

void GameEngine::applyChoice(const Choice &choice, bool /*combatSuccess*/) {
    m_player.applyHpDelta(choice.hpDelta);
    m_player.applyMoraleDelta(choice.moraleDelta);
    for (const QString &flag : choice.grantedFlags) {
        m_player.setFlag(flag);
        emit flagSet(flag);
    }
    emit statsChanged(m_player.hp, m_player.morale);
}

void GameEngine::navigateTo(const QString &nodeId) {
    if (!m_currentScenario) return;
    const StoryNode *node = m_currentScenario->getNode(nodeId);
    if (!node) return;

    m_player.currentNodeId = nodeId;

    if (node->isVictory) {
        // 解锁下一场景
        int nextId = static_cast<int>(m_player.currentScenario) + 1;
        if (nextId <= static_cast<int>(ScenarioId::Berlin)) {
            m_player.unlockScenario(static_cast<ScenarioId>(nextId));
        }
        emit nodeChanged(node);
        emit scenarioVictory(m_player.currentScenario);
        return;
    }

    if (node->isDefeat) {
        emit nodeChanged(node);
        emit scenarioDefeat(m_player.currentScenario);
        return;
    }

    emit nodeChanged(node);

    // Narrative 类型自动提供继续按钮（由 UI 处理）
}

int GameEngine::rollDice(const QList<PlayerClass> &bonusClasses) const {
    int roll = QRandomGenerator::global()->bounded(1, 101); // [1,100]
    if (bonusClasses.contains(m_player.playerClass))
        roll += 20;
    return qBound(1, roll, 120);
}

bool GameEngine::isGameOver() const {
    return m_player.isDead();
}

bool GameEngine::isVictory() const {
    const StoryNode *node = currentNode();
    return node && node->isVictory;
}

bool GameEngine::isInCombat() const {
    const StoryNode *node = currentNode();
    if (!node) return false;
    for (const Choice &c : node->choices)
        if (c.isCombat) return true;
    return false;
}
