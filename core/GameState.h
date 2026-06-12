#pragma once
#include <QString>
#include <QList>
#include <QStringList>

// ============================================================
//  核心枚举 — 职业、场景、游戏状态
// ============================================================

enum class PlayerClass {
    Infantry      = 0,   // 步兵
    TankCrew      = 1,   // 坦克兵
    FighterPilot  = 2,   // 战斗机飞行员
    BomberPilot   = 3,   // 轰炸机飞行员
    SubmarineCrew = 4,   // 潜艇驾驶员
    BattleshipCrew= 5    // 战列舰驾驶员
};

enum class ScenarioId {
    FallGelb    = 0,    // 黄色方案     1940年5月
    Britain     = 1,    // 不列颠空战   1940年7月-10月
    WolfPack    = 2,    // 群狼海战     1941年-1943年
    Stalingrad  = 3,    // 斯大林格勒   1942年-1943年
    Berlin      = 4     // 柏林战役     1945年4月
};

// std::unordered_map 的哈希函数
struct ScenarioIdHash {
    size_t operator()(ScenarioId id) const noexcept {
        return static_cast<size_t>(id);
    }
};

enum class GameScreen {
    MainMenu,
    CharacterCreate,
    ScenarioSelect,
    Playing,
    GameOver
};

enum class NodeType {
    Narrative,   // 纯叙事，单个"继续"按钮
    Choice,      // 玩家做出选择
    Combat,      // 骰子判定战斗
    Ending       // 胜利/失败终止节点
};

// ============================================================
//  职业/场景辅助函数
// ============================================================

inline QString playerClassName(PlayerClass cls) {
    switch (cls) {
        case PlayerClass::Infantry:       return QStringLiteral("步兵");
        case PlayerClass::TankCrew:       return QStringLiteral("坦克兵");
        case PlayerClass::FighterPilot:   return QStringLiteral("战斗机飞行员");
        case PlayerClass::BomberPilot:    return QStringLiteral("轰炸机飞行员");
        case PlayerClass::SubmarineCrew:  return QStringLiteral("潜艇驾驶员");
        case PlayerClass::BattleshipCrew: return QStringLiteral("战列舰驾驶员");
        default:                          return QStringLiteral("未知");
    }
}

inline QString scenarioName(ScenarioId id) {
    switch (id) {
        case ScenarioId::FallGelb:   return QStringLiteral("黄色方案");
        case ScenarioId::Britain:    return QStringLiteral("不列颠空战");
        case ScenarioId::WolfPack:   return QStringLiteral("群狼海战");
        case ScenarioId::Stalingrad: return QStringLiteral("斯大林格勒战役");
        case ScenarioId::Berlin:     return QStringLiteral("柏林战役");
        default:                     return QStringLiteral("未知场景");
    }
}

inline QString scenarioYear(ScenarioId id) {
    switch (id) {
        case ScenarioId::FallGelb:   return QStringLiteral("1940年5月");
        case ScenarioId::Britain:    return QStringLiteral("1940年7月");
        case ScenarioId::WolfPack:   return QStringLiteral("1941年");
        case ScenarioId::Stalingrad: return QStringLiteral("1942年8月");
        case ScenarioId::Berlin:     return QStringLiteral("1945年4月");
        default:                     return {};
    }
}

// 判断某职业是否可参与某场景
inline bool classAllowedInScenario(PlayerClass cls, ScenarioId scen) {
    switch (scen) {
        case ScenarioId::FallGelb:
        case ScenarioId::Stalingrad:
            return cls == PlayerClass::Infantry || cls == PlayerClass::TankCrew;
        case ScenarioId::Britain:
            return cls == PlayerClass::FighterPilot || cls == PlayerClass::BomberPilot;
        case ScenarioId::WolfPack:
            return cls == PlayerClass::SubmarineCrew || cls == PlayerClass::BattleshipCrew;
        case ScenarioId::Berlin:
            return true; // 所有职业参与最终决战
        default:
            return false;
    }
}
