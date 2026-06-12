#pragma once
#include <QObject>
#include <unordered_map>
#include <memory>
#include "Player.h"
#include "ScenarioBase.h"
#include "StoryNode.h"

// ============================================================
//  GameEngine — 核心游戏逻辑引擎
// ============================================================
class GameEngine : public QObject {
    Q_OBJECT
public:
    explicit GameEngine(QObject *parent = nullptr);

    // ---- 初始化 ----
    void newGame(const QString &playerName, PlayerClass cls);
    void loadGame(const Player &savedPlayer);

    // ---- 场景导航 ----
    bool startScenario(ScenarioId id);
    const StoryNode* currentNode()    const;
    const Player&    player()         const { return m_player; }
    Player&          playerMutable()        { return m_player; }
    ScenarioId       currentScenario() const { return m_player.currentScenario; }

    // ---- 选择处理 ----
    // 返回 true 表示游戏继续，false 表示需要UI更新但无进一步操作
    bool makeChoice(int choiceIndex);

    // ---- 骰子系统 ----
    // 返回 [0,100]，加职业加成后与 threshold 比较
    int rollDice(const QList<PlayerClass> &bonusClasses) const;

    // ---- 状态查询 ----
    bool isGameOver()  const;
    bool isVictory()   const;
    bool isInCombat()  const;

signals:
    void nodeChanged(const StoryNode *node);
    void statsChanged(int hp, int morale);
    void scenarioVictory(ScenarioId id);
    void scenarioDefeat(ScenarioId id);
    void combatResult(bool success, int hpChange, int moraleChange);
    void flagSet(const QString &flag);

private:
    Player  m_player;
    // 使用 std::unordered_map 而非 QMap，因为 Qt 6.10+ 的 QMap 底层用 std::map，无法存储 std::unique_ptr（不可拷贝）
    std::unordered_map<ScenarioId, std::unique_ptr<ScenarioBase>, ScenarioIdHash> m_scenarios;
    ScenarioBase *m_currentScenario = nullptr;

    void registerScenarios();
    void applyChoice(const Choice &choice, bool combatSuccess = true);
    void navigateTo(const QString &nodeId);
};
