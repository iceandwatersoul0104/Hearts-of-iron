#pragma once
#include <QObject>
#include <QString>
#include <QList>
#include "Player.h"

// ============================================================
//  存档信息摘要（用于存档列表显示）
// ============================================================
struct SaveInfo {
    int     slot        = -1;
    bool    isAutoSave  = false;
    QString playerName;
    QString className;
    QString scenarioName;
    QString timestamp;
    bool    valid       = false;
};

// ============================================================
//  SaveManager — 存档管理
//  slot 0 = 自动存档，slot 1-3 = 手动存档
// ============================================================
class SaveManager : public QObject {
    Q_OBJECT
public:
    explicit SaveManager(QObject *parent = nullptr);

    static const int AUTO_SLOT    = 0;
    static const int MAX_MANUAL   = 3;

    // 保存
    bool saveGame(int slot, const Player &player);
    bool autoSave(const Player &player);

    // 读取
    bool   loadGame(int slot, Player &outPlayer);

    // 列出所有存档
    QList<SaveInfo> listSaves() const;
    SaveInfo        slotInfo(int slot) const;

    // 删除
    bool deleteSlot(int slot);

private:
    QString savePath(int slot) const;
    QString saveDir()          const;
};
