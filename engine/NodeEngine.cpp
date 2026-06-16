#include "NodeEngine.h"
#include "DlcManager.h"
#include <QDebug>

NodeEngine::NodeEngine(DiceSystem *dice, QObject *parent)
    : QObject(parent), m_dice(dice) {}

bool NodeEngine::startDlc(const DlcManifest &manifest, const QString &dlcBasePath,
                           PlayerSystem &player) {
    m_manifest = manifest;
    m_dlcBasePath = dlcBasePath;
    m_player = &player;
    m_loadedChapters.clear();

    return startChapter(player.currentChapter.isEmpty()
                        ? manifest.startChapter
                        : player.currentChapter);
}

bool NodeEngine::startChapter(const QString &chapterId) {
    if (!loadChapterInternal(chapterId))
        return false;

    m_player->currentChapter = chapterId;
    m_player->resetStats();

    // 优先使用职业专属起始节点，否则使用默认起始节点
    QString startId;
    if (m_currentChapter.classStartNodes.contains(m_player->classId)) {
        startId = m_currentChapter.classStartNodes[m_player->classId];
    } else {
        startId = m_currentChapter.startNodeId;
    }
    navigateTo(startId);
    return true;
}

bool NodeEngine::loadChapterInternal(const QString &chapterId) {
    if (m_loadedChapters.contains(chapterId)) {
        m_currentChapter = m_loadedChapters[chapterId];
        return true;
    }

    const DlcChapterMeta *meta = nullptr;
    for (const auto &ch : m_manifest.chapters) {
        if (ch.id == chapterId) { meta = &ch; break; }
    }
    if (!meta) return false;

    DlcManager loader;
    DlcChapter chapter;
    if (!loader.loadChapter(m_dlcBasePath, meta->file, chapter))
        return false;

    m_loadedChapters[chapterId] = chapter;
    m_currentChapter = chapter;
    return true;
}

const StoryNode *NodeEngine::currentNode() const {
    auto it = m_currentChapter.nodes.find(m_player->currentNodeId);
    if (it != m_currentChapter.nodes.end())
        return &it.value();
    return nullptr;
}

bool NodeEngine::makeChoice(int choiceIndex) {
    const StoryNode *node = currentNode();
    if (!node) return false;

    // Narrative 节点：直接跳转
    if (node->type == NodeType::Narrative) {
        navigateTo(node->nextNodeId);
        return true;
    }

    if (choiceIndex < 0 || choiceIndex >= node->choices.size())
        return false;

    const Choice &choice = node->choices[choiceIndex];

    // 职业限制检查
    if (choice.classRestricted &&
        !choice.allowedClasses.contains(m_player->classId))
        return false;

    // 标志条件检查
    for (const QString &flag : choice.requiredFlags) {
        if (!m_player->hasFlag(flag)) return false;
    }

    if (choice.isCombat) {
        // Qt6: QStringList inherits QList<QString>, convert explicitly
        QStringList bonusList;
        for (const QString &cls : choice.bonusClasses)
            bonusList.append(cls);
        int roll = m_dice->rollWithBonus(bonusList, m_player->classId);
        bool success = DiceSystem::checkSuccess(roll, choice.combatThreshold);

        if (success) {
            applyChoice(choice);
            emit combatResult(true, choice.hpDelta, choice.moraleDelta);
            navigateTo(choice.successNodeId);
        } else {
            m_player->applyHpDelta(choice.failHpDelta);
            m_player->applyMoraleDelta(choice.failMoraleDelta);
            for (const QString &flag : choice.grantedFlags) {
                m_player->setFlag(flag);
                emit flagSet(flag);
            }
            emit combatResult(false, choice.failHpDelta, choice.failMoraleDelta);
            emit statsChanged(m_player->hp, m_player->morale);

            if (m_player->isDead()) {
                QString defeatId = m_currentChapter.defeatNodeId;
                if (defeatId.isEmpty())
                    defeatId = QStringLiteral("__defeat__");
                navigateTo(defeatId);
                return true;
            }
            navigateTo(choice.failureNodeId);
        }
    } else {
        applyChoice(choice);
        navigateTo(choice.nextNodeId);
    }
    return true;
}

void NodeEngine::applyChoice(const Choice &choice) {
    m_player->applyHpDelta(choice.hpDelta);
    m_player->applyMoraleDelta(choice.moraleDelta);
    for (const QString &flag : choice.grantedFlags) {
        m_player->setFlag(flag);
        emit flagSet(flag);
    }
    emit statsChanged(m_player->hp, m_player->morale);
}

void NodeEngine::navigateTo(const QString &nodeId) {
    auto it = m_currentChapter.nodes.find(nodeId);
    if (it == m_currentChapter.nodes.end()) return;

    const StoryNode *node = &it.value();
    m_player->currentNodeId = nodeId;

    if (node->isVictory) {
        // 线性解锁下一章节
        const QList<DlcChapterMeta> &chapters = m_manifest.chapters;
        for (int i = 0; i < chapters.size(); ++i) {
            if (chapters[i].id == m_player->currentChapter) {
                if (i + 1 < chapters.size()) {
                    QString nextChId = chapters[i + 1].id;
                    m_player->unlockChapter(nextChId);
                }
                break;
            }
        }
        emit nodeChanged(node);
        emit chapterVictory(m_player->currentChapter);
        return;
    }

    if (node->isDefeat) {
        emit nodeChanged(node);
        emit chapterDefeat(m_player->currentChapter);
        return;
    }

    emit nodeChanged(node);
}

bool NodeEngine::isGameOver() const {
    return m_player->isDead();
}

bool NodeEngine::isVictory() const {
    const StoryNode *node = currentNode();
    return node && node->isVictory;
}

bool NodeEngine::isInCombat() const {
    const StoryNode *node = currentNode();
    if (!node) return false;
    for (const Choice &c : node->choices)
        if (c.isCombat) return true;
    return false;
}
