#pragma once
#include <QDialog>
#include <QList>
#include "../core/SaveManager.h"

class QPushButton;
class QLabel;
class QButtonGroup;

class SaveLoadDialog : public QDialog {
    Q_OBJECT
public:
    // savingMode: true = 保存模式, false = 读取模式
    SaveLoadDialog(bool savingMode, SaveManager *saveManager, Player *player, QWidget *parent = nullptr);
    ~SaveLoadDialog() = default;

signals:
    void gameLoaded(const Player &loadedPlayer);

private slots:
    void onSlotSelected(int id);
    void onActionTriggered();
    void onDeleteTriggered();

private:
    void setupUi();
    void refreshSlots();

    bool m_savingMode;
    SaveManager *m_saveManager;
    Player *m_player; // 用于在保存模式下读取当前玩家状态以写入存档

    QButtonGroup *m_buttonGroup = nullptr;
    QPushButton *m_actionBtn = nullptr;
    QPushButton *m_deleteBtn = nullptr;
    QPushButton *m_cancelBtn = nullptr;

    int m_selectedSlot = -1;

    struct SlotUI {
        QWidget *card = nullptr;
        QLabel *titleLabel = nullptr;
        QLabel *infoLabel = nullptr;
        QLabel *timeLabel = nullptr;
    };
    QList<SlotUI> m_slotsUI;
};
