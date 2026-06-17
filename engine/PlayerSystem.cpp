#include "PlayerSystem.h"

PlayerSystem::PlayerSystem(const QString &n, const QString &cid, const QString &did)
    : name(n), classId(cid), dlcId(did) {}

void PlayerSystem::applyHpDelta(int delta) {
    hp = std::clamp(hp + delta, 0, maxHp);
}

void PlayerSystem::applyMoraleDelta(int delta) {
    morale = std::clamp(morale + delta, 0, maxMorale);
}

void PlayerSystem::applyMpDelta(int delta) {
    mp = std::clamp(mp + delta, 0, maxMp);
}

bool PlayerSystem::chapterUnlocked(const QString &chId) const {
    return unlockedChapters.contains(chId);
}

void PlayerSystem::unlockChapter(const QString &chId) {
    unlockedChapters.insert(chId);
}

// --- RPG 装备/物品操作 ---

void PlayerSystem::equipItem(const QString &slot, const QString &item) {
    equipment[slot] = item;
}

void PlayerSystem::unequipItem(const QString &slot) {
    equipment.remove(slot);
}

QString PlayerSystem::equipmentAt(const QString &slot) const {
    return equipment.value(slot, QString());
}

void PlayerSystem::addItem(const QString &item) {
    inventory.append(item);
}

void PlayerSystem::removeItem(const QString &item) {
    inventory.removeOne(item);
}

bool PlayerSystem::hasItem(const QString &item) const {
    return inventory.contains(item);
}

void PlayerSystem::setQuickItem(int slot, const QString &item) {
    while (quickItems.size() <= slot)
        quickItems.append(QString());
    quickItems[slot] = item;
}

QString PlayerSystem::quickItemAt(int slot) const {
    if (slot >= 0 && slot < quickItems.size())
        return quickItems[slot];
    return QString();
}

// --- 序列化 ---

void PlayerSystem::resetStats() {
    hp     = maxHp;
    morale = maxMorale;
    mp     = maxMp;
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

    // RPG fields
    obj["mp"]              = mp;
    obj["maxMp"]           = maxMp;
    obj["level"]           = level;
    obj["exp"]             = exp;
    obj["baseStr"]         = baseStr;
    obj["baseAgi"]         = baseAgi;
    obj["baseInt"]         = baseInt;

    QJsonObject eqObj;
    for (auto it = equipment.begin(); it != equipment.end(); ++it)
        eqObj[it.key()] = it.value();
    obj["equipment"] = eqObj;

    QJsonArray invArr;
    for (const QString &item : inventory) invArr.append(item);
    obj["inventory"] = invArr;

    QJsonArray qiArr;
    for (const QString &item : quickItems) qiArr.append(item);
    obj["quickItems"] = qiArr;

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

    // RPG fields
    mp              = obj["mp"].toInt(50);
    maxMp           = obj["maxMp"].toInt(50);
    level           = obj["level"].toInt(1);
    exp             = obj["exp"].toInt(0);
    baseStr         = obj["baseStr"].toInt(10);
    baseAgi         = obj["baseAgi"].toInt(10);
    baseInt         = obj["baseInt"].toInt(10);

    equipment.clear();
    QJsonObject eqObj = obj["equipment"].toObject();
    for (auto it = eqObj.begin(); it != eqObj.end(); ++it)
        equipment[it.key()] = it.value().toString();

    inventory.clear();
    for (const auto &v : obj["inventory"].toArray())
        inventory.append(v.toString());

    quickItems.clear();
    for (const auto &v : obj["quickItems"].toArray())
        quickItems.append(v.toString());

    currentChapter  = obj["currentChapter"].toString();
    currentNodeId   = obj["currentNodeId"].toString();

    flags.clear();
    for (const auto &v : obj["flags"].toArray())
        flags.insert(v.toString());

    unlockedChapters.clear();
    for (const auto &v : obj["unlockedChapters"].toArray())
        unlockedChapters.insert(v.toString());
}
