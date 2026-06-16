#include "PlayerSystem.h"

PlayerSystem::PlayerSystem(const QString &n, const QString &cid, const QString &did)
    : name(n), classId(cid), dlcId(did) {}

void PlayerSystem::applyHpDelta(int delta) {
    hp = std::clamp(hp + delta, 0, maxHp);
}

void PlayerSystem::applyMoraleDelta(int delta) {
    morale = std::clamp(morale + delta, 0, maxMorale);
}

bool PlayerSystem::chapterUnlocked(const QString &chId) const {
    return unlockedChapters.contains(chId);
}

void PlayerSystem::unlockChapter(const QString &chId) {
    unlockedChapters.insert(chId);
}

void PlayerSystem::resetStats() {
    hp     = maxHp;
    morale = maxMorale;
    flags.clear();
    currentNodeId.clear();
}

QJsonObject PlayerSystem::toJson() const {
    QJsonObject obj;
    obj["engineVersion"]   = QStringLiteral("2.0");
    obj["dlcId"]           = dlcId;
    obj["playerName"]      = name;
    obj["playerClass"]     = classId;
    obj["hp"]              = hp;
    obj["maxHp"]           = maxHp;
    obj["morale"]          = morale;
    obj["maxMorale"]       = maxMorale;
    obj["currentChapter"]  = currentChapter;
    obj["currentNodeId"]   = currentNodeId;

    QJsonArray flagArr;
    for (const QString &f : flags) flagArr.append(f);
    obj["flags"] = flagArr;

    QJsonArray unlockedArr;
    for (const QString &ch : unlockedChapters) unlockedArr.append(ch);
    obj["unlockedChapters"] = unlockedArr;

    return obj;
}

void PlayerSystem::fromJson(const QJsonObject &obj) {
    dlcId           = obj["dlcId"].toString();
    name            = obj["playerName"].toString();
    classId         = obj["playerClass"].toString();
    hp              = obj["hp"].toInt(100);
    maxHp           = obj["maxHp"].toInt(100);
    morale          = obj["morale"].toInt(100);
    maxMorale       = obj["maxMorale"].toInt(100);
    currentChapter  = obj["currentChapter"].toString();
    currentNodeId   = obj["currentNodeId"].toString();

    flags.clear();
    for (const auto &v : obj["flags"].toArray())
        flags.insert(v.toString());

    unlockedChapters.clear();
    for (const auto &v : obj["unlockedChapters"].toArray())
        unlockedChapters.insert(v.toString());
}
