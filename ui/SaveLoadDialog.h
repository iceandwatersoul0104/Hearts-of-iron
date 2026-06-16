#pragma once
#include <QDialog>
#include <QList>
#include "../engine/SaveManager.h"

class QPushButton;
class QLabel;

class SaveLoadDialog : public QDialog {
    Q_OBJECT
public:
    // savingMode: true = 保存模式, false = 读取模式
    SaveLoadDialog(bool savingMode, SaveManager *saveManager, PlayerSystem *player, QWidget *parent = nullptr);
    ~SaveLoadDialog() = default;

    void setDlcInfo(const QString &dlcTitle, const QString &className,
                    const QString &chapterName);

signals:
    void gameLoaded(const PlayerSystem &loadedPlayer);

protected:
    bool eventFilter(QObject *obj, QEvent *event) override;

private slots:
    void onSlotSelected(int id);
    void onActionTriggered();
    void onDeleteTriggered();

private:
    void setupUi();
    void refreshSlots();

    bool m_savingMode;
    SaveManager *m_saveManager;
    PlayerSystem *m_player;

    QPushButton *m_actionBtn = nullptr;
    QPushButton *m_deleteBtn = nullptr;
    QPushButton *m_cancelBtn = nullptr;

    int m_selectedSlot = -1;

    QString m_dlcTitle;
    QString m_className;
    QString m_chapterName;

    struct SlotUI {
        QWidget *card = nullptr;
        QLabel *titleLabel = nullptr;
        QLabel *infoLabel = nullptr;
        QLabel *timeLabel = nullptr;
    };
    QList<SlotUI> m_slotsUI;
};
