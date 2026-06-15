#pragma once
#include <QObject>
#include <QStringList>

class DiceSystem : public QObject {
    Q_OBJECT
public:
    explicit DiceSystem(QObject *parent = nullptr);

    // 基础 D100 掷骰 [1, 100]
    int roll() const;

    // 带职业加成的掷骰
    // bonusClasses: 享有加成的职业ID列表
    // playerClass:   当前玩家职业ID
    // 若 playerClass 在 bonusClasses 中，则 +20
    int rollWithBonus(const QStringList &bonusClasses,
                      const QString &playerClass) const;

    // 判定成功：roll >= threshold
    static bool checkSuccess(int roll, int threshold);
};
