#pragma once
#include <QString>
#include <QList>
#include <QMap>
#include <QSet>

// ============================================================
//  DlcTypes — 引擎运行时数据结构（不依赖任何具体 DLC）
// ============================================================

// --- 共享枚举 ---
enum class NodeType {
    Narrative,   // 纯叙事，单个"继续"按钮
    Choice,      // 玩家做出选择
    Ending       // 胜利/失败终止节点
};

enum class GameScreen {
    MainMenu,
    DlcSelect,
    CharacterCreate,
    Playing,
    GameOver
};

// --- 运行时数据结构 ---

struct DlcClass {
    QString id;       // 字符串ID，如 "infantry"
    QString name;     // 显示名称，如 "步兵"
    QString desc;     // 描述文本
};

struct DlcChapterMeta {
    QString id;       // 如 "fall_gelb"
    QString file;     // JSON 文件相对路径
    QString name;     // 显示名称
    QString subtitle; // 副标题
    QString unlock;   // 解锁条件："start" 或 chapter id
};

struct DlcManifest {
    QString dlcId;
    QString title;
    QString subtitle;
    QString author;
    QString version;
    QList<DlcClass> classes;
    QList<DlcChapterMeta> chapters;
    QString startChapter;
    QMap<QString, QString> music;   // key → 相对文件路径
    bool valid = true;
    QStringList errors;
};

// --- Choice（职业字段改为 QString） ---
struct Choice {
    QString text;
    QString nextNodeId;

    int hpDelta      = 0;
    int moraleDelta  = 0;

    QSet<QString> requiredFlags;
    QSet<QString> grantedFlags;

    bool classRestricted = false;
    QList<QString> allowedClasses;      // ← 改为 QString

    bool isCombat        = false;
    int  combatThreshold = 50;
    QList<QString> bonusClasses;        // ← 改为 QString
    QString successNodeId;
    QString failureNodeId;
    int failHpDelta      = -20;
    int failMoraleDelta  = -15;
};

// --- StoryNode ---
struct StoryNode {
    QString id;
    QString locationTitle;
    QString text;

    NodeType type = NodeType::Choice;

    QList<Choice> choices;

    QString nextNodeId;
    QString musicKey;
    bool isVictory = false;
    bool isDefeat  = false;

    QMap<QString, QString> classText;

    QString textFor(const QString &classId) const {
        if (classText.contains(classId))
            return classText[classId];
        return text;
    }
};

// --- DlcChapter ---
struct DlcChapter {
    QString chapterId;
    QString startNodeId;           // 默认起始节点 (classStartNodes 中未匹配时使用)
    QString defeatNodeId;
    QMap<QString, QString> classStartNodes; // 职业专属起始节点映射: classId → nodeId
    QMap<QString, StoryNode> nodes;
};
