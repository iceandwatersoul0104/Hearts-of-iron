#include "Player.h"
#include <QJsonArray>
#include <algorithm>

Player::Player(const QString &n, PlayerClass cls)
    : name(n), playerClass(cls)
{
    unlockedScenarios.insert(static_cast<int>(ScenarioId::FallGelb));
}

void Player::applyHpDelta(int delta) {
    hp = std::clamp(hp + delta, 0, maxHp);
}

void Player::applyMoraleDelta(int delta) {
    morale = std::clamp(morale + delta, 0, maxMorale);
}

bool Player::scenarioUnlocked(ScenarioId id) const {
    return unlockedScenarios.contains(static_cast<int>(id));
}

void Player::unlockScenario(ScenarioId id) {
    unlockedScenarios.insert(static_cast<int>(id));
}

void Player::resetStats() {
    hp     = maxHp;
    morale = maxMorale;
    flags.clear();
    currentNodeId.clear();
}

QJsonObject Player::toJson() const {
    QJsonObject obj;
    obj["name"]             = name;
    obj["playerClass"]      = static_cast<int>(playerClass);
    obj["hp"]               = hp;
    obj["maxHp"]            = maxHp;
    obj["morale"]           = morale;
    obj["maxMorale"]        = maxMorale;
    obj["currentScenario"]  = static_cast<int>(currentScenario);
    obj["currentNodeId"]    = currentNodeId;

    QJsonArray flagArr;
    for (const QString &f : flags) flagArr.append(f);
    obj["flags"] = flagArr;

    QJsonArray unlockedArr;
    for (int id : unlockedScenarios) unlockedArr.append(id);
    obj["unlockedScenarios"] = unlockedArr;

    return obj;
}

void Player::fromJson(const QJsonObject &obj) {
    name            = obj["name"].toString();
    playerClass     = static_cast<PlayerClass>(obj["playerClass"].toInt());
    hp              = obj["hp"].toInt(100);
    maxHp           = obj["maxHp"].toInt(100);
    morale          = obj["morale"].toInt(100);
    maxMorale       = obj["maxMorale"].toInt(100);
    currentScenario = static_cast<ScenarioId>(obj["currentScenario"].toInt());
    currentNodeId   = obj["currentNodeId"].toString();

    flags.clear();
    for (const auto &v : obj["flags"].toArray())
        flags.insert(v.toString());

    unlockedScenarios.clear();
    for (const auto &v : obj["unlockedScenarios"].toArray())
        unlockedScenarios.insert(v.toInt());
}
