#include "SaveManager.h"
#include <QStandardPaths>
#include <QDir>
#include <QFile>
#include <QJsonDocument>
#include <QJsonObject>
#include <QDateTime>

SaveManager::SaveManager(QObject *parent) : QObject(parent) {
    QDir().mkpath(saveDir());
}

QString SaveManager::saveDir() const {
    return QStandardPaths::writableLocation(QStandardPaths::AppDataLocation)
           + "/saves";
}

QString SaveManager::savePath(int slot) const {
    return saveDir() + QString("/save_%1.json").arg(slot);
}

bool SaveManager::saveGame(int slot, const PlayerSystem &player,
                            const QString &dlcTitle,
                            const QString &className,
                            const QString &chapterName) {
    QJsonObject root;
    root["slot"]        = slot;
    root["autoSave"]    = (slot == AUTO_SLOT);
    root["timestamp"]   = QDateTime::currentDateTime().toString("yyyy-MM-dd HH:mm:ss");
    root["dlcTitle"]    = dlcTitle;
    root["className"]   = className;
    root["chapterName"] = chapterName;
    root["player"]      = player.toJson();

    QJsonDocument doc(root);
    QFile file(savePath(slot));
    if (!file.open(QIODevice::WriteOnly | QIODevice::Truncate))
        return false;
    file.write(doc.toJson(QJsonDocument::Indented));
    return true;
}

bool SaveManager::autoSave(const PlayerSystem &player,
                            const QString &dlcTitle,
                            const QString &className,
                            const QString &chapterName) {
    return saveGame(AUTO_SLOT, player, dlcTitle, className, chapterName);
}

bool SaveManager::loadGame(int slot, PlayerSystem &outPlayer) {
    QFile file(savePath(slot));
    if (!file.open(QIODevice::ReadOnly)) return false;

    QJsonDocument doc = QJsonDocument::fromJson(file.readAll());
    if (doc.isNull() || !doc.isObject()) return false;

    QJsonObject root = doc.object();
    if (!root.contains("player")) return false;

    outPlayer.fromJson(root["player"].toObject());
    return true;
}

QList<SaveInfo> SaveManager::listSaves() const {
    QList<SaveInfo> result;
    for (int slot = 0; slot <= MAX_MANUAL; ++slot)
        result.append(slotInfo(slot));
    return result;
}

SaveInfo SaveManager::slotInfo(int slot) const {
    SaveInfo info;
    info.slot = slot;
    info.isAutoSave = (slot == AUTO_SLOT);

    QFile file(savePath(slot));
    if (!file.open(QIODevice::ReadOnly)) { info.valid = false; return info; }

    QJsonDocument doc = QJsonDocument::fromJson(file.readAll());
    if (doc.isNull()) { info.valid = false; return info; }

    QJsonObject root = doc.object();
    QJsonObject playerObj = root["player"].toObject();

    info.valid       = true;
    info.dlcId       = playerObj["dlcId"].toString();
    info.dlcTitle    = root["dlcTitle"].toString();
    info.playerName  = playerObj["playerName"].toString();
    info.className   = root["className"].toString();
    info.chapterName = root["chapterName"].toString();
    info.timestamp   = root["timestamp"].toString();
    return info;
}

bool SaveManager::deleteSlot(int slot) {
    return QFile::remove(savePath(slot));
}
