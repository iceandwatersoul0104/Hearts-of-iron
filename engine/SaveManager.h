#pragma once
#include <QObject>
#include <QString>
#include <QList>
#include <QDir>
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

    void setCategory(const QString &category) { m_category = category; QDir().mkpath(saveDir()); }
    QString category() const { return m_category; }

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
    QString m_category;
};
