#pragma once
#include <QString>
#include <QList>
#include <QSet>
#include <QMap>
#include "GameState.h"

// ============================================================
//  Choice — 单个选项
// ============================================================
struct Choice {
    QString text;               // 按钮显示文字
    QString nextNodeId;         // 默认跳转节点 (非战斗选项用)

    // 资源变化（立即生效，战斗前）
    int hpDelta      = 0;
    int moraleDelta  = 0;

    // 叙事标志
    QSet<QString> requiredFlags;   // 需要拥有这些标志才能看到此选项
    QSet<QString> grantedFlags;    // 选择后设置的标志

    // 职业限制
    bool classRestricted = false;
    QList<PlayerClass> allowedClasses;

    // ---- 战斗型选项 ----
    bool isCombat        = false;
    int  combatThreshold = 50;     // roll >= threshold → 成功
    QList<PlayerClass> bonusClasses; // 这些职业骰子+20
    QString successNodeId;
    QString failureNodeId;
    int failHpDelta      = -20;
    int failMoraleDelta  = -15;
};

// ============================================================
//  StoryNode — 单个故事节点
// ============================================================
struct StoryNode {
    QString id;
    QString locationTitle;  // 顶部标题栏，如 "1940年5月10日 · 阿登森林"
    QString text;           // 叙事正文（纯文本，支持\n分段）

    NodeType type = NodeType::Choice;

    QList<Choice> choices;

    // Narrative 类型直接跳转
    QString nextNodeId;

    // 音乐提示键 (对应 MusicPlayer 中的 key)
    QString musicKey;

    // 终止节点标志
    bool isVictory = false;
    bool isDefeat  = false;

    // 职业专属文本覆盖
    QMap<PlayerClass, QString> classText;

    // 辅助：根据职业获取实际显示文本
    QString textFor(PlayerClass cls) const {
        if (classText.contains(cls))
            return classText[cls];
        return text;
    }
};
