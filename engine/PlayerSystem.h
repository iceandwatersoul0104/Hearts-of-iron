#pragma once
#include <QString>
#include <QSet>
#include <QMap>
#include <QStringList>
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

    // === RPG 专属字段（剧情模式不使用） ===
    int     mp        = 50;       // 魔力值
    int     maxMp     = 50;       // 最大魔力
    int     level     = 1;        // 等级
    int     exp       = 0;        // 经验值
    int     baseStr   = 10;       // 力量
    int     baseAgi   = 10;       // 敏捷
    int     baseInt   = 10;       // 智力
    QMap<QString, QString> equipment;   // 装备栏: slot → itemName
    QStringList inventory;              // 背包物品
    QStringList quickItems;             // 快捷道具 (最多4个)

    // 叙事标志
    QSet<QString> flags;

    // 进度
    QString   currentChapter;
    QString   currentNodeId;
    QSet<QString> unlockedChapters;

    // 操作方法
    void applyHpDelta(int delta);
    void applyMoraleDelta(int delta);
    void applyMpDelta(int delta);
    bool isAlive()    const { return hp > 0; }
    bool hasMorale()  const { return morale > 0; }
    bool isDead()     const { return !isAlive() || !hasMorale(); }

    bool hasFlag(const QString &flag) const { return flags.contains(flag); }
    void setFlag(const QString &flag)       { flags.insert(flag); }

    bool chapterUnlocked(const QString &chId) const;
    void unlockChapter(const QString &chId);

    // RPG 装备/物品操作
    void equipItem(const QString &slot, const QString &item);
    void unequipItem(const QString &slot);
    QString equipmentAt(const QString &slot) const;
    void addItem(const QString &item);
    void removeItem(const QString &item);
    bool hasItem(const QString &item) const;
    void setQuickItem(int slot, const QString &item);
    QString quickItemAt(int slot) const;

    // 序列化
    QJsonObject toJson()       const;
    void        fromJson(const QJsonObject &obj);

    // 重置
    void resetStats();
};
