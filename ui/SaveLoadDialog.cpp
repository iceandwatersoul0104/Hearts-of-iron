#include "SaveLoadDialog.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>
#include <QLabel>
#include <QButtonGroup>
#include <QMessageBox>
#include <QStyle>
#include <QDateTime>

SaveLoadDialog::SaveLoadDialog(bool savingMode, SaveManager *saveManager, Player *player, QWidget *parent)
    : QDialog(parent), m_savingMode(savingMode), m_saveManager(saveManager), m_player(player)
{
    setupUi();
    refreshSlots();
}

void SaveLoadDialog::setupUi() {
    setWindowTitle(m_savingMode ? QStringLiteral("保存战役") : QStringLiteral("继续战役"));
    setMinimumSize(480, 520);
    resize(500, 560);
    
    // 强制使用无边框窗口或者保持原生对话框样式，通过样式表定制
    setObjectName(QStringLiteral("SaveLoadDialog"));

    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(20, 20, 20, 20);
    mainLayout->setSpacing(15);

    // 顶部标题
    QLabel *titleLabel = new QLabel(m_savingMode ? QStringLiteral("选择要覆盖的战役存档：") : QStringLiteral("选择要继续的战役记录："), this);
    titleLabel->setObjectName(QStringLiteral("dialogTitle"));
    mainLayout->addWidget(titleLabel);

    // 槽位容器布局
    QVBoxLayout *slotsLayout = new QVBoxLayout();
    slotsLayout->setSpacing(10);

    m_buttonGroup = new QButtonGroup(this);
    m_buttonGroup->setExclusive(true);

    for (int i = 0; i < 4; ++i) {
        QWidget *card = new QWidget(this);
        card->setObjectName(QStringLiteral("slotCard"));
        card->setCursor(Qt::PointingHandCursor);

        QHBoxLayout *cardLayout = new QHBoxLayout(card);
        cardLayout->setContentsMargins(15, 10, 15, 10);

        QVBoxLayout *textLayout = new QVBoxLayout();
        textLayout->setSpacing(4);

        QLabel *sTitle = new QLabel(card);
        sTitle->setObjectName(QStringLiteral("slotTitle"));
        
        QLabel *sInfo = new QLabel(card);
        sInfo->setObjectName(QStringLiteral("slotInfo"));
        
        QLabel *sTime = new QLabel(card);
        sTime->setObjectName(QStringLiteral("slotTime"));

        textLayout->addWidget(sTitle);
        textLayout->addWidget(sInfo);
        textLayout->addWidget(sTime);
        cardLayout->addLayout(textLayout);

        // 如果是保存模式且为自动存档位 (Slot 0)，使卡片外观变灰色或不可交互
        if (m_savingMode && i == SaveManager::AUTO_SLOT) {
            card->setEnabled(false);
            card->setToolTip(QStringLiteral("自动存档不支持手动覆盖"));
        }

        slotsLayout->addWidget(card);

        SlotUI ui;
        ui.card = card;
        ui.titleLabel = sTitle;
        ui.infoLabel = sInfo;
        ui.timeLabel = sTime;
        m_slotsUI.append(ui);

        // 使用事件过滤器或重写 mousePressEvent 进行卡片选择
        // 简易起见，利用子组件拦截点击，或将 card 包装为 QPushButton 自定义绘制。
        // 这里直接利用 QPushButton 的覆层，或者直接通过一个透明的 QPushButton 覆盖在 card 上作为触发器
        QPushButton *trigger = new QPushButton(card);
        trigger->setObjectName(QStringLiteral("slotTrigger"));
        trigger->setGeometry(0, 0, 500, 100); // 运行时 resizeEvent 调整，或直接使用卡片作为按钮
        trigger->setFlat(true);
        trigger->setStyleSheet(QStringLiteral("background: transparent; border: none;"));
        m_buttonGroup->addButton(trigger, i);

        // 当卡片被点击时触发
        connect(trigger, &QPushButton::clicked, this, [this, i]() {
            onSlotSelected(i);
        });
    }
    mainLayout->addLayout(slotsLayout);

    // 底部控制按钮
    QHBoxLayout *ctrlLayout = new QHBoxLayout();
    ctrlLayout->setSpacing(12);

    m_actionBtn = new QPushButton(m_savingMode ? QStringLiteral("保存记录") : QStringLiteral("开始读取"), this);
    m_actionBtn->setObjectName(QStringLiteral("dialogActionBtn"));
    m_actionBtn->setEnabled(false);
    connect(m_actionBtn, &QPushButton::clicked, this, &SaveLoadDialog::onActionTriggered);

    m_deleteBtn = new QPushButton(QStringLiteral("删除记录"), this);
    m_deleteBtn->setObjectName(QStringLiteral("dialogDeleteBtn"));
    m_deleteBtn->setEnabled(false);
    connect(m_deleteBtn, &QPushButton::clicked, this, &SaveLoadDialog::onDeleteTriggered);

    m_cancelBtn = new QPushButton(QStringLiteral("取消"), this);
    m_cancelBtn->setObjectName(QStringLiteral("dialogCancelBtn"));
    connect(m_cancelBtn, &QPushButton::clicked, this, &QDialog::reject);

    ctrlLayout->addWidget(m_deleteBtn);
    ctrlLayout->addStretch();
    ctrlLayout->addWidget(m_cancelBtn);
    ctrlLayout->addWidget(m_actionBtn);

    mainLayout->addLayout(ctrlLayout);
}

void SaveLoadDialog::refreshSlots() {
    QList<SaveInfo> infos = m_saveManager->listSaves();
    for (int i = 0; i < 4; ++i) {
        const SaveInfo &info = infos[i];
        SlotUI &ui = m_slotsUI[i];

        // 自动缩放触发器大小
        QPushButton *trigger = qobject_cast<QPushButton*>(m_buttonGroup->button(i));
        if (trigger) {
            trigger->resize(ui.card->size());
        }

        QString titleText = (i == SaveManager::AUTO_SLOT) ? QStringLiteral("【自动存档】") : QString::asprintf("【战役存档 %d】", i);
        ui.titleLabel->setText(titleText);

        if (info.valid) {
            ui.infoLabel->setText(QStringLiteral("军官: %1 | 兵种: %2 | 战役: %3")
                                  .arg(info.playerName, info.className, info.scenarioName));
            ui.timeLabel->setText(QStringLiteral("记录时间: %1").arg(info.timestamp));
            ui.card->setProperty("empty", false);
        } else {
            ui.infoLabel->setText(QStringLiteral("战区通讯中断 — 无存档数据"));
            ui.timeLabel->setText(QStringLiteral("-"));
            ui.card->setProperty("empty", true);
            
            // 如果是读取模式，无数据的槽位不可选择
            if (!m_savingMode) {
                m_buttonGroup->button(i)->setEnabled(false);
                ui.card->setEnabled(false);
            }
        }
        
        // 刷新 QSS
        ui.card->style()->unpolish(ui.card);
        ui.card->style()->polish(ui.card);
    }
}

void SaveLoadDialog::onSlotSelected(int id) {
    m_selectedSlot = id;

    // 更新选中卡片视觉样式
    for (int i = 0; i < 4; ++i) {
        QWidget *card = m_slotsUI[i].card;
        card->setProperty("selected", (i == id));
        card->style()->unpolish(card);
        card->style()->polish(card);
    }

    // 自动判断对应存档有效性
    QList<SaveInfo> infos = m_saveManager->listSaves();
    bool slotHasData = (id >= 0 && id < 4) ? infos[id].valid : false;

    // 只有非自动存档，且该槽位有数据，才可删除
    m_deleteBtn->setEnabled(id != SaveManager::AUTO_SLOT && slotHasData);
    
    // 如果是保存模式：非自动存档位即可保存
    // 如果是读取模式：被选中的槽位本身有数据才可读取
    if (m_savingMode) {
        m_actionBtn->setEnabled(id != SaveManager::AUTO_SLOT);
    } else {
        m_actionBtn->setEnabled(slotHasData);
    }
}

void SaveLoadDialog::onActionTriggered() {
    if (m_selectedSlot < 0) return;

    if (m_savingMode) {
        // 保存逻辑
        if (m_saveManager->saveGame(m_selectedSlot, *m_player)) {
            QMessageBox::information(this, QStringLiteral("存档成功"), QStringLiteral("战役记录已成功保存。"));
            accept();
        } else {
            QMessageBox::warning(this, QStringLiteral("错误"), QStringLiteral("无法保存战役，请检查磁盘权限。"));
        }
    } else {
        // 读取逻辑
        Player loadedPlayer;
        if (m_saveManager->loadGame(m_selectedSlot, loadedPlayer)) {
            emit gameLoaded(loadedPlayer);
            accept();
        } else {
            QMessageBox::warning(this, QStringLiteral("错误"), QStringLiteral("存档读取失败或数据损坏。"));
        }
    }
}

void SaveLoadDialog::onDeleteTriggered() {
    if (m_selectedSlot < 0 || m_selectedSlot == SaveManager::AUTO_SLOT) return;

    auto res = QMessageBox::question(this, QStringLiteral("删除确认"),
                                     QStringLiteral("确认要彻底抹去该战役记录吗？这无法恢复。"),
                                     QMessageBox::Yes | QMessageBox::No);
    if (res == QMessageBox::Yes) {
        m_saveManager->deleteSlot(m_selectedSlot);
        m_selectedSlot = -1;
        m_deleteBtn->setEnabled(false);
        m_actionBtn->setEnabled(false);
        
        // 重置选中属性
        for (int i = 0; i < 4; ++i) {
            m_slotsUI[i].card->setProperty("selected", false);
        }
        
        refreshSlots();
    }
}
