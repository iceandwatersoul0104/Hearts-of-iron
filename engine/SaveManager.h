#pragma once
#include <QObject>
#include <QString>
#include <QList>
#include "PlayerSystem.h"

struct SaveInfo {
    int     slot        = -1;
    bool    isAutoSave  = false;
    QString dlcId;
    QString dlcTitle;
    QString playerName;
    QString className;
    QString chapterName;
    QString timestamp;
    bool    valid       = false;
};

class SaveManager : public QObject {
    Q_OBJECT
public:
    explicit SaveManager(QObject *parent = nullptr);

    static const int AUTO_SLOT    = 0;
    static const int MAX_MANUAL   = 3;

    bool saveGame(int slot, const PlayerSystem &player,
                  const QString &dlcTitle = {},
                  const QString &className = {},
                  const QString &chapterName = {});
    bool autoSave(const PlayerSystem &player,
                  const QString &dlcTitle = {},
                  const QString &className = {},
                  const QString &chapterName = {});

    bool   loadGame(int slot, PlayerSystem &outPlayer);
    QList<SaveInfo> listSaves() const;
    SaveInfo        slotInfo(int slot) const;
    bool deleteSlot(int slot);

private:
    QString savePath(int slot) const;
    QString saveDir()          const;
};
