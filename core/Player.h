#pragma once
#include <QString>
#include <QSet>
#include <QJsonObject>
#include "GameState.h"

// ============================================================
//  Player — 玩家角色数据
// ============================================================
class Player {
public:
    Player() = default;
    Player(const QString &name, PlayerClass cls);

    // ---- 基础属性 ----
    QString     name;
    PlayerClass playerClass = PlayerClass::Infantry;
    int         hp          = 100;
    int         maxHp       = 100;
    int         morale      = 100;
    int         maxMorale   = 100;

    // ---- 叙事标志集合 ----
    QSet<QString> flags;

    // ---- 进度 ----
    ScenarioId  currentScenario  = ScenarioId::FallGelb;
    QString     currentNodeId;
    QSet<int>   unlockedScenarios; // ScenarioId 的整数值集合

    // ---- 操作方法 ----
    void applyHpDelta(int delta);
    void applyMoraleDelta(int delta);
    bool isAlive()    const { return hp > 0; }
    bool hasMorale()  const { return morale > 0; }
    bool isDead()     const { return !isAlive() || !hasMorale(); }

    bool hasFlag(const QString &flag) const { return flags.contains(flag); }
    void setFlag(const QString &flag)       { flags.insert(flag); }

    bool scenarioUnlocked(ScenarioId id) const;
    void unlockScenario(ScenarioId id);

    // ---- 序列化 ----
    QJsonObject toJson()       const;
    void        fromJson(const QJsonObject &obj);

    // ---- 重置到新场景 ----
    void resetStats();
};
