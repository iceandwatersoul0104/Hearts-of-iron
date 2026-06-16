#pragma once
#include <QString>
#include <QSet>
#include <QJsonObject>
#include <QJsonArray>
#include <algorithm>
#include "DlcTypes.h"

class PlayerSystem {
public:
    PlayerSystem() = default;
    PlayerSystem(const QString &name, const QString &classId, const QString &dlcId);

    // 基础属性
    QString name;
    QString classId;       // 字符串ID，如 "infantry"
    QString dlcId;         // 所属 DLC
    int     hp        = 100;
    int     maxHp     = 100;
    int     morale    = 100;
    int     maxMorale = 100;

    // 叙事标志
    QSet<QString> flags;

    // 进度
    QString   currentChapter;
    QString   currentNodeId;
    QSet<QString> unlockedChapters;

    // 操作方法
    void applyHpDelta(int delta);
    void applyMoraleDelta(int delta);
    bool isAlive()    const { return hp > 0; }
    bool hasMorale()  const { return morale > 0; }
    bool isDead()     const { return !isAlive() || !hasMorale(); }

    bool hasFlag(const QString &flag) const { return flags.contains(flag); }
    void setFlag(const QString &flag)       { flags.insert(flag); }

    bool chapterUnlocked(const QString &chId) const;
    void unlockChapter(const QString &chId);

    // 序列化
    QJsonObject toJson()       const;
    void        fromJson(const QJsonObject &obj);

    // 重置
    void resetStats();
};
