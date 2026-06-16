#include "DiceSystem.h"
#include <QRandomGenerator>
#include <algorithm>

DiceSystem::DiceSystem(QObject *parent) : QObject(parent) {}

int DiceSystem::roll() const {
    return QRandomGenerator::global()->bounded(1, 101); // [1, 100]
}

int DiceSystem::rollWithBonus(const QStringList &bonusClasses,
                              const QString &playerClass) const {
    int result = roll();
    if (bonusClasses.contains(playerClass))
        result += 20;
    return qBound(1, result, 120);
}

bool DiceSystem::checkSuccess(int roll, int threshold) {
    return roll >= threshold;
}
