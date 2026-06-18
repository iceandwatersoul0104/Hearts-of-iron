#include "RpgGameWidget.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGridLayout>
#include <QPushButton>
#include <QLabel>
#include <QProgressBar>
#include <QTextBrowser>
#include <QScrollBar>
#include <QScrollArea>
#include <QFrame>
#include <QEvent>
#include <QMenu>
#include <QDialog>
#include <QDialogButtonBox>
#include <QMessageBox>

RpgGameWidget::RpgGameWidget(QWidget *parent)
    : QWidget(parent)
{
    setupUi();
}

void RpgGameWidget::setupUi() {
    setObjectName(QStringLiteral("RpgGameWidget"));

    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(15, 10, 15, 10);
    mainLayout->setSpacing(10);

    // ===== Header =====
    QWidget *header = new QWidget(this);
    header->setObjectName(QStringLiteral("gameHeader"));
    QHBoxLayout *headerLayout = new QHBoxLayout(header);
    headerLayout->setContentsMargins(12, 8, 12, 8);
    headerLayout->setSpacing(20);

    QVBoxLayout *infoLayout = new QVBoxLayout();
    infoLayout->setSpacing(2);
    m_nameLabel = new QLabel(QStringLiteral("勇者: ---"), header);
    m_nameLabel->setObjectName(QStringLiteral("headerNameLabel"));
    m_classLabel = new QLabel(QStringLiteral("职业: ---"), header);
    m_classLabel->setObjectName(QStringLiteral("headerClassLabel"));
    infoLayout->addWidget(m_nameLabel);
    infoLayout->addWidget(m_classLabel);
    headerLayout->addLayout(infoLayout);

    QVBoxLayout *levelLayout = new QVBoxLayout();
    levelLayout->setSpacing(2);
    m_levelLabel = new QLabel(QStringLiteral("Lv.1"), header);
    m_levelLabel->setStyleSheet("font-size: 16px; font-weight: bold; color: #ff9900;");
    m_expLabel = new QLabel(QStringLiteral("EXP: 0"), header);
    m_expLabel->setStyleSheet("font-size: 11px; color: #aaa;");
    levelLayout->addWidget(m_levelLabel);
    levelLayout->addWidget(m_expLabel);
    headerLayout->addLayout(levelLayout);
    headerLayout->addStretch();

    QVBoxLayout *scenLayout = new QVBoxLayout();
    scenLayout->setSpacing(2);
    QLabel *scenLabel = new QLabel(QStringLiteral(""), header);
    scenLabel->setObjectName(QStringLiteral("headerScenarioLabel"));
    scenLabel->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
    QLabel *locLabel = new QLabel(QStringLiteral(""), header);
    locLabel->setObjectName(QStringLiteral("headerLocationLabel"));
    locLabel->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
    scenLayout->addWidget(scenLabel);
    scenLayout->addWidget(locLabel);
    headerLayout->addLayout(scenLayout);

    mainLayout->addWidget(header);

    // ===== Content: Sidebar + Story =====
    QHBoxLayout *contentLayout = new QHBoxLayout();
    contentLayout->setSpacing(10);

    QScrollArea *sidebarScroll = new QScrollArea(this);
    sidebarScroll->setFixedWidth(250);
    sidebarScroll->setWidgetResizable(true);
    sidebarScroll->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    sidebarScroll->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    sidebarScroll->setStyleSheet("QScrollArea { border: none; background: #181818; } QScrollBar:vertical { width: 6px; background: #111; } QScrollBar::handle:vertical { background: #444; border-radius: 3px; }");

    QWidget *sidebar = new QWidget();
    sidebar->setObjectName(QStringLiteral("gameHeader"));
    QVBoxLayout *sideLayout = new QVBoxLayout(sidebar);
    sideLayout->setContentsMargins(10, 8, 10, 8);
    sideLayout->setSpacing(8);
    sidebarScroll->setWidget(sidebar);

    // HP
    QLabel *hpTitle = new QLabel(QStringLiteral("❤ 生命值 HP"), sidebar);
    hpTitle->setStyleSheet("color: #ff6666; font-weight: bold; font-size: 12px;");
    sideLayout->addWidget(hpTitle);
    m_hpBar = new QProgressBar(sidebar);
    m_hpBar->setObjectName(QStringLiteral("gameHpBar"));
    m_hpBar->setRange(0, 100);
    m_hpBar->setValue(100);
    m_hpBar->setFormat("%v/%m");
    m_hpBar->setFixedHeight(18);
    sideLayout->addWidget(m_hpBar);
    m_hpLabel = new QLabel(QStringLiteral("100 / 100"), sidebar);
    m_hpLabel->setStyleSheet("color: #ff9999; font-size: 10px;");
    m_hpLabel->setAlignment(Qt::AlignRight);
    sideLayout->addWidget(m_hpLabel);

    // MP
    QLabel *mpTitle = new QLabel(QStringLiteral("💎 魔力值 MP"), sidebar);
    mpTitle->setStyleSheet("color: #6699ff; font-weight: bold; font-size: 12px;");
    sideLayout->addWidget(mpTitle);
    m_mpBar = new QProgressBar(sidebar);
    m_mpBar->setObjectName(QStringLiteral("gameMoraleBar"));
    m_mpBar->setRange(0, 100);
    m_mpBar->setValue(50);
    m_mpBar->setFormat("%v/%m");
    m_mpBar->setFixedHeight(18);
    sideLayout->addWidget(m_mpBar);
    m_mpLabel = new QLabel(QStringLiteral("50 / 50"), sidebar);
    m_mpLabel->setStyleSheet("color: #99bbff; font-size: 10px;");
    m_mpLabel->setAlignment(Qt::AlignRight);
    sideLayout->addWidget(m_mpLabel);

    // Separator
    QFrame *sep1 = new QFrame(sidebar);
    sep1->setFrameShape(QFrame::HLine);
    sep1->setStyleSheet("color: #444;");
    sideLayout->addWidget(sep1);

    // Equipment (clickable buttons)
    QLabel *eqTitle = new QLabel(QStringLiteral("⚔ 装备栏 (点击更换)"), sidebar);
    eqTitle->setStyleSheet("color: #ffcc00; font-weight: bold; font-size: 12px;");
    sideLayout->addWidget(eqTitle);

    m_weaponBtn = new QPushButton(QStringLiteral("🗡 武器: 无"), sidebar);
    m_weaponBtn->setCursor(Qt::PointingHandCursor);
    m_weaponBtn->setProperty("eqSlot", "weapon");
    m_weaponBtn->setStyleSheet("QPushButton { text-align: left; padding: 4px 8px; background: #1a1a2e; border: 1px solid #335; color: #ccf; border-radius: 4px; font-size: 11px; } QPushButton:hover { background: #222244; border-color: #66f; }");
    connect(m_weaponBtn, &QPushButton::clicked, this, &RpgGameWidget::onEquipSlotClicked);
    sideLayout->addWidget(m_weaponBtn);

    m_armorBtn = new QPushButton(QStringLiteral("🛡 防具: 无"), sidebar);
    m_armorBtn->setCursor(Qt::PointingHandCursor);
    m_armorBtn->setProperty("eqSlot", "armor");
    m_armorBtn->setStyleSheet("QPushButton { text-align: left; padding: 4px 8px; background: #1a1a2e; border: 1px solid #335; color: #ccf; border-radius: 4px; font-size: 11px; } QPushButton:hover { background: #222244; border-color: #66f; }");
    connect(m_armorBtn, &QPushButton::clicked, this, &RpgGameWidget::onEquipSlotClicked);
    sideLayout->addWidget(m_armorBtn);

    m_accessoryBtn = new QPushButton(QStringLiteral("💍 饰品: 无"), sidebar);
    m_accessoryBtn->setCursor(Qt::PointingHandCursor);
    m_accessoryBtn->setProperty("eqSlot", "accessory");
    m_accessoryBtn->setStyleSheet("QPushButton { text-align: left; padding: 4px 8px; background: #1a1a2e; border: 1px solid #335; color: #ccf; border-radius: 4px; font-size: 11px; } QPushButton:hover { background: #222244; border-color: #66f; }");
    connect(m_accessoryBtn, &QPushButton::clicked, this, &RpgGameWidget::onEquipSlotClicked);
    sideLayout->addWidget(m_accessoryBtn);

    // Separator
    QFrame *sep2 = new QFrame(sidebar);
    sep2->setFrameShape(QFrame::HLine);
    sep2->setStyleSheet("color: #444;");
    sideLayout->addWidget(sep2);

    // Quick Items
    QLabel *qiTitle = new QLabel(QStringLiteral("⚡ 快捷道具 (点击使用)"), sidebar);
    qiTitle->setStyleSheet("color: #66ff66; font-weight: bold; font-size: 12px;");
    sideLayout->addWidget(qiTitle);

    QWidget *quickPanel = new QWidget(sidebar);
    QGridLayout *qiLayout = new QGridLayout(quickPanel);
    qiLayout->setContentsMargins(2, 2, 2, 2);
    qiLayout->setSpacing(4);
    for (int i = 0; i < 4; i++) {
        m_quickBtns[i] = new QPushButton(QStringLiteral("空"), quickPanel);
        m_quickBtns[i]->setCursor(Qt::PointingHandCursor);
        m_quickBtns[i]->setProperty("quickSlot", i);
        m_quickBtns[i]->setMinimumHeight(32);
        m_quickBtns[i]->setStyleSheet(
            "QPushButton { font-size: 10px; background: #1a2e1a; border: 1px solid #353; color: #aca; border-radius: 4px; }"
            "QPushButton:hover { background: #224422; border-color: #6f6; color: #cfc; }"
            "QPushButton:disabled { background: #1a1a1a; color: #444; border-color: #222; }"
        );
        connect(m_quickBtns[i], &QPushButton::clicked, this, &RpgGameWidget::onQuickItemClicked);
        qiLayout->addWidget(m_quickBtns[i], i / 2, i % 2);
    }
    sideLayout->addWidget(quickPanel);

    // Open Backpack Dialog button
    QPushButton *backpackBtn = new QPushButton(QStringLiteral("📦 打开背包"), sidebar);
    backpackBtn->setCursor(Qt::PointingHandCursor);
    backpackBtn->setMinimumHeight(32);
    backpackBtn->setStyleSheet(
        "QPushButton { font-size: 12px; font-weight: bold; background: #2a1a0a; border: 1px solid #654; color: #daa520; border-radius: 4px; }"
        "QPushButton:hover { background: #3a2a10; border-color: #daa520; }"
    );
    connect(backpackBtn, &QPushButton::clicked, this, [this]() { showBackpackDialog(); });
    sideLayout->addWidget(backpackBtn);

    // Character stats display
    QLabel *statTitle = new QLabel(QStringLiteral("📊 角色属性"), sidebar);
    statTitle->setStyleSheet("color: #ffcc00; font-weight: bold; font-size: 12px;");
    sideLayout->addWidget(statTitle);
    m_strLabel = new QLabel(sidebar);
    m_strLabel->setStyleSheet("color: #ddd; font-size: 10px; padding: 0 4px;");
    sideLayout->addWidget(m_strLabel);
    m_agiLabel = new QLabel(sidebar);
    m_agiLabel->setStyleSheet("color: #ddd; font-size: 10px; padding: 0 4px;");
    sideLayout->addWidget(m_agiLabel);
    m_intLabel = new QLabel(sidebar);
    m_intLabel->setStyleSheet("color: #ddd; font-size: 10px; padding: 0 4px;");
    sideLayout->addWidget(m_intLabel);

    m_invPanel = new QWidget(sidebar);
    m_invLayout = new QVBoxLayout(m_invPanel);
    m_invLayout->setContentsMargins(4, 2, 4, 2);
    m_invLayout->setSpacing(2);
    m_invPanel->setVisible(false);
    sideLayout->addWidget(m_invPanel);

    sideLayout->addStretch();
    contentLayout->addWidget(sidebarScroll);

    // Story text
    m_textBrowser = new QTextBrowser(this);
    m_textBrowser->setObjectName(QStringLiteral("gameTextBrowser"));
    m_textBrowser->setReadOnly(true);
    m_textBrowser->setFocusPolicy(Qt::NoFocus);
    m_textBrowser->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
    m_textBrowser->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    m_textBrowser->viewport()->installEventFilter(this);
    m_textBrowser->installEventFilter(this);
    contentLayout->addWidget(m_textBrowser, 1);

    mainLayout->addLayout(contentLayout, 1);

    // ===== Bottom =====
    QHBoxLayout *bottomLayout = new QHBoxLayout();
    bottomLayout->setSpacing(15);

    m_optionsContainer = new QWidget(this);
    m_optionsLayout = new QVBoxLayout(m_optionsContainer);
    m_optionsLayout->setContentsMargins(0, 0, 0, 0);
    m_optionsLayout->setSpacing(6);
    bottomLayout->addWidget(m_optionsContainer, 3);

    QWidget *ctrlPanel = new QWidget(this);
    ctrlPanel->setObjectName(QStringLiteral("gameCtrlPanel"));
    QVBoxLayout *ctrlLayout = new QVBoxLayout(ctrlPanel);
    ctrlLayout->setContentsMargins(8, 8, 8, 8);
    ctrlLayout->setSpacing(6);

    m_saveBtn = new QPushButton(QStringLiteral("保存冒险"), ctrlPanel);
    m_saveBtn->setObjectName(QStringLiteral("gameCtrlBtn"));
    m_saveBtn->setCursor(Qt::PointingHandCursor);
    m_saveBtn->setMinimumSize(100, 32);
    connect(m_saveBtn, &QPushButton::clicked, this, &RpgGameWidget::saveClicked);

    m_loadBtn = new QPushButton(QStringLiteral("载入冒险"), ctrlPanel);
    m_loadBtn->setObjectName(QStringLiteral("gameCtrlBtn"));
    m_loadBtn->setCursor(Qt::PointingHandCursor);
    m_loadBtn->setMinimumSize(100, 32);
    connect(m_loadBtn, &QPushButton::clicked, this, &RpgGameWidget::loadClicked);

    m_exitBtn = new QPushButton(QStringLiteral("退出冒险"), ctrlPanel);
    m_exitBtn->setObjectName(QStringLiteral("gameCtrlBtn"));
    m_exitBtn->setCursor(Qt::PointingHandCursor);
    m_exitBtn->setMinimumSize(100, 32);
    connect(m_exitBtn, &QPushButton::clicked, this, &RpgGameWidget::exitClicked);

    ctrlLayout->addWidget(m_saveBtn);
    ctrlLayout->addWidget(m_loadBtn);
    ctrlLayout->addWidget(m_exitBtn);
    bottomLayout->addWidget(ctrlPanel, 1);

    mainLayout->addLayout(bottomLayout);

    m_typeTimer = new QTimer(this);
    m_typeTimer->setInterval(20);
    connect(m_typeTimer, &QTimer::timeout, this, &RpgGameWidget::onTypeTimerTick);
}

// ===== Public Methods =====

void RpgGameWidget::setSkills(const QList<Skill> &skills) {
    m_skills = skills;
}

void RpgGameWidget::updatePlayerStats(int hp, int maxHp, int mp, int maxMp) {
    m_hpBar->setRange(0, maxHp);
    m_hpBar->setValue(hp);
    m_hpLabel->setText(QStringLiteral("%1 / %2").arg(hp).arg(maxHp));
    m_mpBar->setRange(0, maxMp);
    m_mpBar->setValue(mp);
    m_mpLabel->setText(QStringLiteral("%1 / %2").arg(mp).arg(maxMp));

    // Update stat display with equipment bonuses
    if (m_player) {
        int eqStr = 0, eqAgi = 0, eqInt = 0, eqHp = 0, eqMp = 0;
        QString wpn = m_player->equipmentAt("weapon");
        QString arm = m_player->equipmentAt("armor");
        QString acc = m_player->equipmentAt("accessory");
        if (!wpn.isEmpty() && wpn != QStringLiteral("无")) {
            int s, a, i, h, m; getEquipStats(wpn, s, a, i, h, m);
            eqStr += s; eqAgi += a; eqInt += i; eqHp += h; eqMp += m;
        }
        if (!arm.isEmpty() && arm != QStringLiteral("无")) {
            int s, a, i, h, m; getEquipStats(arm, s, a, i, h, m);
            eqStr += s; eqAgi += a; eqInt += i; eqHp += h; eqMp += m;
        }
        if (!acc.isEmpty() && acc != QStringLiteral("无")) {
            int s, a, i, h, m; getEquipStats(acc, s, a, i, h, m);
            eqStr += s; eqAgi += a; eqInt += i; eqHp += h; eqMp += m;
        }
        m_strLabel->setText(QStringLiteral("力量: %1 (+%2)").arg(m_player->baseStr).arg(eqStr));
        m_agiLabel->setText(QStringLiteral("敏捷: %1 (+%2)").arg(m_player->baseAgi).arg(eqAgi));
        m_intLabel->setText(QStringLiteral("智力: %1 (+%2)").arg(m_player->baseInt).arg(eqInt));
    }
}

void RpgGameWidget::updatePlayerInfo(const QString &name, const QString &className,
                                      int level, int exp) {
    m_nameLabel->setText(QStringLiteral("勇者: %1").arg(name));
    m_classLabel->setText(QStringLiteral("职业: %1").arg(className));
    m_levelLabel->setText(QStringLiteral("Lv.%1").arg(level));
    m_expLabel->setText(QStringLiteral("EXP: %1").arg(exp));
}

void RpgGameWidget::updateEquipment(const QMap<QString, QString> &equipment) {
    QString w = equipment.value("weapon", QStringLiteral("无"));
    QString a = equipment.value("armor", QStringLiteral("无"));
    QString ac = equipment.value("accessory", QStringLiteral("无"));
    m_weaponBtn->setText(QStringLiteral("🗡 武器: %1").arg(w));
    m_armorBtn->setText(QStringLiteral("🛡 防具: %1").arg(a));
    m_accessoryBtn->setText(QStringLiteral("💍 饰品: %1").arg(ac));
}

void RpgGameWidget::updateQuickItems(const QStringList &quickItems) {
    for (int i = 0; i < 4; i++) {
        if (i < quickItems.size() && !quickItems[i].isEmpty()) {
            m_quickBtns[i]->setText(quickItems[i]);
            m_quickBtns[i]->setEnabled(true);
        } else {
            m_quickBtns[i]->setText(QStringLiteral("空"));
            m_quickBtns[i]->setEnabled(false);
        }
    }
}

void RpgGameWidget::updateInventory(const QStringList &inventory) {
    rebuildInventoryPanel();
}

void RpgGameWidget::rebuildInventoryPanel() {
    QLayoutItem *child;
    while ((child = m_invLayout->takeAt(0)) != nullptr) {
        if (child->widget()) child->widget()->deleteLater();
        delete child;
    }
    if (m_player && !m_player->inventory.isEmpty()) {
        for (int i = 0; i < m_player->inventory.size(); i++) {
            const QString &item = m_player->inventory[i];
            QHBoxLayout *row = new QHBoxLayout();
            row->setSpacing(4);

            QLabel *itemLabel = new QLabel(QStringLiteral("• %1").arg(item), m_invPanel);
            itemLabel->setStyleSheet("color: #bbb; font-size: 10px; padding: 2px 4px;");
            row->addWidget(itemLabel, 1);

            // Quick slot button
            QPushButton *quickBtn = new QPushButton(QStringLiteral("设快捷"), m_invPanel);
            quickBtn->setFixedSize(50, 20);
            quickBtn->setCursor(Qt::PointingHandCursor);
            quickBtn->setStyleSheet("QPushButton { font-size: 9px; background: #1a2e1a; border: 1px solid #353; color: #8a8; border-radius: 2px; } QPushButton:hover { background: #224422; }");
            int idx = i;
            connect(quickBtn, &QPushButton::clicked, this, [this, idx]() {
                if (!m_player || idx >= m_player->inventory.size()) return;
                // Set to first empty quick slot
                for (int s = 0; s < 4; s++) {
                    if (s >= m_player->quickItems.size() || m_player->quickItems[s].isEmpty()) {
                        m_player->setQuickItem(s, m_player->inventory[idx]);
                        updateQuickItems(m_player->quickItems);
                        QMessageBox::information(this, QStringLiteral("快捷设置"),
                            QStringLiteral("已将 %1 设到快捷槽 %2").arg(m_player->inventory[idx]).arg(s + 1));
                        break;
                    }
                }
            });
            row->addWidget(quickBtn);

            // Equip button (for equipment-type items)
            if (item.startsWith(QStringLiteral("武器:")) || item.startsWith(QStringLiteral("防具:")) || item.startsWith(QStringLiteral("饰品:"))) {
                QPushButton *eqBtn = new QPushButton(QStringLiteral("装备"), m_invPanel);
                eqBtn->setFixedSize(40, 20);
                eqBtn->setCursor(Qt::PointingHandCursor);
                eqBtn->setStyleSheet("QPushButton { font-size: 9px; background: #2e1a1a; border: 1px solid #533; color: #a88; border-radius: 2px; } QPushButton:hover { background: #442222; }");
                connect(eqBtn, &QPushButton::clicked, this, [this, idx]() {
                    if (!m_player || idx >= m_player->inventory.size()) return;
                    QString item = m_player->inventory[idx];
                    QString slot, prefix;
                    if (item.startsWith(QStringLiteral("武器:"))) { slot = "weapon"; prefix = QStringLiteral("武器:"); }
                    else if (item.startsWith(QStringLiteral("防具:"))) { slot = "armor"; prefix = QStringLiteral("防具:"); }
                    else if (item.startsWith(QStringLiteral("饰品:"))) { slot = "accessory"; prefix = QStringLiteral("饰品:"); }
                    else return;

                    // Unequip current first
                    QString cur = m_player->equipmentAt(slot);
                    if (!cur.isEmpty() && cur != QStringLiteral("无")) {
                        m_player->addItem(prefix + cur);
                    }
                    // Equip new
                    m_player->removeItem(item);
                    m_player->equipItem(slot, item.mid(prefix.length()));
                    updateEquipment(m_player->equipment);
                    updateInventory(m_player->inventory);
                });
                row->addWidget(eqBtn);
            }

            m_invLayout->addLayout(row);
        }
    } else {
        QLabel *empty = new QLabel(QStringLiteral("（背包为空）"), m_invPanel);
        empty->setStyleSheet("color: #555; font-size: 10px;");
        m_invLayout->addWidget(empty);
    }
}

// ===== Story Node =====

void RpgGameWidget::showStoryNode(const StoryNode *node, const QString &playerClassId,
                                   const QString &chapterName, bool isLastChapter) {
    if (!node) return;

    m_playerClassId = playerClassId;
    m_isLastChapter = isLastChapter;
    m_pendingChoiceIdx = -1;

    QLabel *scenLabel = findChild<QLabel*>("headerScenarioLabel");
    QLabel *locLabel  = findChild<QLabel*>("headerLocationLabel");
    if (scenLabel) scenLabel->setText(chapterName);
    if (locLabel)  locLabel->setText(node->locationTitle);

    QString displayText = node->textFor(playerClassId);
    if (displayText.isEmpty()) displayText = node->text;

    m_fullText = displayText;
    m_charIndex = 0;
    m_isTyping = true;
    m_textBrowser->clear();

    clearOptionButtons();
    showStoryNode_impl(node);

    m_saveBtn->setEnabled(false);
    m_loadBtn->setEnabled(false);
    m_typeTimer->start();
}

void RpgGameWidget::showStoryNode_impl(const StoryNode *node) {
    bool isEnding = (node->type == NodeType::Ending || node->isVictory || node->isDefeat);

    if (isEnding) {
        if (node->isVictory && !m_isLastChapter) {
            QPushButton *nextBtn = new QPushButton(QStringLiteral("【进入下一章】"), m_optionsContainer);
            nextBtn->setObjectName(QStringLiteral("choiceOptionBtn"));
            nextBtn->setCursor(Qt::PointingHandCursor);
            nextBtn->setMinimumHeight(38);
            nextBtn->setProperty("choiceIndex", -2);
            nextBtn->setVisible(false);
            connect(nextBtn, &QPushButton::clicked, this, &RpgGameWidget::onChoiceButtonClicked);
            m_optionsLayout->addWidget(nextBtn);
        } else if (node->isVictory) {
            QPushButton *endBtn = new QPushButton(QStringLiteral("【返回首页 — 传说的终结】"), m_optionsContainer);
            endBtn->setObjectName(QStringLiteral("choiceOptionBtn"));
            endBtn->setCursor(Qt::PointingHandCursor);
            endBtn->setMinimumHeight(38);
            endBtn->setVisible(false);
            connect(endBtn, &QPushButton::clicked, this, &RpgGameWidget::exitClicked);
            m_optionsLayout->addWidget(endBtn);
        } else if (node->isDefeat) {
            QPushButton *defBtn = new QPushButton(QStringLiteral("【返回首页 — 重新挑战】"), m_optionsContainer);
            defBtn->setObjectName(QStringLiteral("combatOptionBtn"));
            defBtn->setCursor(Qt::PointingHandCursor);
            defBtn->setMinimumHeight(38);
            defBtn->setVisible(false);
            connect(defBtn, &QPushButton::clicked, this, &RpgGameWidget::exitClicked);
            m_optionsLayout->addWidget(defBtn);
        }
        return;
    }

    // Regular nodes: build choice buttons
    int origIdx = 0;
    for (const Choice &choice : node->choices) {
        if (choice.classRestricted && !choice.allowedClasses.contains(m_playerClassId)) {
            origIdx++;
            continue;
        }

        // Check requiredFlags — if missing, show as disabled
        bool flagsMet = true;
        QStringList missingFlags;
        for (const QString &f : choice.requiredFlags) {
            if (!m_player || !m_player->hasFlag(f)) {
                flagsMet = false;
                missingFlags.append(f);
            }
        }

        QString btnText = choice.text;
        if (!flagsMet) {
            // Show what's needed: append requirement hint
            QString flagHint;
            if (missingFlags.contains("hero_holy_blade")) flagHint = QStringLiteral(" [需装备:圣剑]");
            else if (missingFlags.contains("mag_elder_staff")) flagHint = QStringLiteral(" [需装备:长老法杖]");
            else if (missingFlags.contains("arc_elf_bow")) flagHint = QStringLiteral(" [需装备:精灵弓]");
            else if (missingFlags.contains("pri_holy_mace")) flagHint = QStringLiteral(" [需装备:圣光圣典]");
            else if (missingFlags.contains("war_berserk_axe")) flagHint = QStringLiteral(" [需装备:狂战拳套]");
            else flagHint = QStringLiteral(" [未满足条件]");
            btnText += flagHint;
        }

        QPushButton *btn = new QPushButton(btnText, m_optionsContainer);
        btn->setCursor(flagsMet ? Qt::PointingHandCursor : Qt::ForbiddenCursor);
        btn->setMinimumHeight(38);
        btn->setProperty("choiceIndex", origIdx);

        if (!flagsMet) {
            // Disabled — greyed out, not clickable as combat
            btn->setEnabled(false);
            btn->setObjectName(QStringLiteral("gameCtrlBtn"));
        } else {
            btn->setObjectName(choice.isCombat ? QStringLiteral("combatOptionBtn") : QStringLiteral("choiceOptionBtn"));
            if (choice.isCombat)
                btn->setProperty("isCombat", true);
        }

        btn->setVisible(false);
        connect(btn, &QPushButton::clicked, this, &RpgGameWidget::onChoiceButtonClicked);
        m_optionsLayout->addWidget(btn);
        origIdx++;
    }

    // Narrative: continue button
    if (node->type == NodeType::Narrative) {
        QPushButton *continueBtn = new QPushButton(QStringLiteral("【继续】"), m_optionsContainer);
        continueBtn->setObjectName(QStringLiteral("choiceOptionBtn"));
        continueBtn->setCursor(Qt::PointingHandCursor);
        continueBtn->setMinimumHeight(38);
        continueBtn->setProperty("choiceIndex", 0);
        continueBtn->setVisible(false);
        connect(continueBtn, &QPushButton::clicked, this, &RpgGameWidget::onChoiceButtonClicked);
        m_optionsLayout->addWidget(continueBtn);
    }
}

// ===== Typewriter =====

void RpgGameWidget::onTypeTimerTick() {
    if (m_charIndex < m_fullText.length()) {
        QTextCursor cursor = m_textBrowser->textCursor();
        cursor.movePosition(QTextCursor::End);
        int chunk = qMin(3, m_fullText.length() - m_charIndex);
        cursor.insertText(m_fullText.mid(m_charIndex, chunk));
        m_charIndex += chunk;
        QScrollBar *sb = m_textBrowser->verticalScrollBar();
        sb->setValue(sb->maximum());
    } else {
        displayFullText();
    }
}

void RpgGameWidget::displayFullText() {
    m_typeTimer->stop();
    m_isTyping = false;
    m_textBrowser->setPlainText(m_fullText);
    QScrollBar *sb = m_textBrowser->verticalScrollBar();
    sb->setValue(sb->maximum());

    QList<QPushButton*> btns = m_optionsContainer->findChildren<QPushButton*>();
    for (QPushButton *btn : btns)
        btn->setVisible(true);

    m_saveBtn->setEnabled(true);
    m_loadBtn->setEnabled(true);
}

void RpgGameWidget::skipTypewriter() {
    if (m_isTyping)
        displayFullText();
}

void RpgGameWidget::clearOptionButtons() {
    QLayoutItem *child;
    while ((child = m_optionsLayout->takeAt(0)) != nullptr) {
        if (child->widget()) child->widget()->deleteLater();
        delete child;
    }
}

// ===== Event Filter =====

bool RpgGameWidget::eventFilter(QObject *obj, QEvent *event) {
    if (event->type() == QEvent::MouseButtonPress) {
        if (obj == m_textBrowser || obj == m_textBrowser->viewport()) {
            skipTypewriter();
        }
    }
    return QWidget::eventFilter(obj, event);
}

// ===== Button Handlers =====

void RpgGameWidget::onChoiceButtonClicked() {
    QPushButton *btn = qobject_cast<QPushButton*>(sender());
    if (!btn) return;
    int idx = btn->property("choiceIndex").toInt();
    bool isCombat = btn->property("isCombat").toBool();

    if (isCombat && m_player) {
        m_pendingChoiceIdx = idx;
        clearOptionButtons();

        // Check if player can use skills (needs weapon, warrior excepted)
        bool hasWeapon = (m_player->classId == "warrior") ||
                         !m_player->equipmentAt("weapon").isEmpty() &&
                         m_player->equipmentAt("weapon") != QStringLiteral("无");

        // Basic attack (always available)
        QPushButton *basicBtn = new QPushButton(QStringLiteral("【普通攻击】不消耗MP，正常判定"), m_optionsContainer);
        basicBtn->setObjectName(QStringLiteral("combatOptionBtn"));
        basicBtn->setCursor(Qt::PointingHandCursor);
        basicBtn->setMinimumHeight(36);
        basicBtn->setProperty("skillId", QString());
        basicBtn->setVisible(true);
        connect(basicBtn, &QPushButton::clicked, this, &RpgGameWidget::onSkillButtonClicked);
        m_optionsLayout->addWidget(basicBtn);

        // Skill options (only if weapon equipped or warrior)
        if (hasWeapon) {
            for (const Skill &sk : m_skills) {
            if (m_player->level < sk.levelReq) continue;
            QString skillText = QStringLiteral("【%1】(Lv.%2) MP:%3 — %4")
                .arg(sk.name).arg(sk.levelReq).arg(sk.mpCost).arg(sk.desc);
            QPushButton *skBtn = new QPushButton(skillText, m_optionsContainer);
            skBtn->setCursor(Qt::PointingHandCursor);
            skBtn->setMinimumHeight(36);
            skBtn->setProperty("skillId", sk.id);
            skBtn->setVisible(true);
            bool canUse = m_player->mp >= sk.mpCost;
            skBtn->setEnabled(canUse);
            skBtn->setStyleSheet(canUse
                ? "QPushButton { text-align: left; padding: 4px 12px; background: #1a1a3e; border: 1px solid #4488ff; color: #aaccff; border-radius: 4px; font-size: 11px; } QPushButton:hover { background: #222266; border-color: #66aaff; }"
                : "QPushButton { text-align: left; padding: 4px 12px; background: #1a1a1a; border: 1px solid #333; color: #555; border-radius: 4px; font-size: 11px; }");
            connect(skBtn, &QPushButton::clicked, this, &RpgGameWidget::onSkillButtonClicked);
            m_optionsLayout->addWidget(skBtn);
        }
        } else {
            QLabel *noWpn = new QLabel(QStringLiteral("（需要装备武器才能使用技能）"), m_optionsContainer);
            noWpn->setStyleSheet("color: #886666; font-size: 11px; padding: 4px;");
            noWpn->setVisible(true);
            m_optionsLayout->addWidget(noWpn);
        }

        // Cancel button — just do basic combat without modifier
        QPushButton *cancelBtn = new QPushButton(QStringLiteral("【取消】直接战斗（无技能加成）"), m_optionsContainer);
        cancelBtn->setObjectName(QStringLiteral("gameCtrlBtn"));
        cancelBtn->setCursor(Qt::PointingHandCursor);
        cancelBtn->setMinimumHeight(32);
        cancelBtn->setVisible(true);
        connect(cancelBtn, &QPushButton::clicked, this, [this]() {
            emit skillActivated(m_pendingChoiceIdx, 0);
        });
        m_optionsLayout->addWidget(cancelBtn);
    } else {
        emit choiceMade(idx);
    }
}

void RpgGameWidget::onSkillButtonClicked() {
    QPushButton *btn = qobject_cast<QPushButton*>(sender());
    if (!btn || !m_player) return;

    QString skillId = btn->property("skillId").toString();

    if (skillId.isEmpty()) {
        // Basic attack — no skill modifier
        emit skillActivated(m_pendingChoiceIdx, 0);
        return;
    }

    Skill found;
    for (const Skill &sk : m_skills) {
        if (sk.id == skillId) { found = sk; break; }
    }
    if (found.id.isEmpty()) return;

    if (m_player->mp < found.mpCost) {
        QMessageBox::information(this, QStringLiteral("MP不足"),
            QStringLiteral("需要 %1 MP，当前只有 %2 MP").arg(found.mpCost).arg(m_player->mp));
        return;
    }

    // Apply skill effects
    m_player->mp -= found.mpCost;
    if (found.hpRestore > 0) m_player->applyHpDelta(found.hpRestore);
    if (found.mpRestore > 0) m_player->applyMpDelta(found.mpRestore);
    updatePlayerStats(m_player->hp, m_player->maxHp, m_player->mp, m_player->maxMp);

    emit skillActivated(m_pendingChoiceIdx, found.combatBonus);
}

void RpgGameWidget::onQuickItemClicked() {
    QPushButton *btn = qobject_cast<QPushButton*>(sender());
    if (!btn || !m_player) return;

    QString itemName = btn->text();
    if (itemName == QStringLiteral("空")) return;
    int slot = btn->property("quickSlot").toInt();

    // Show context menu
    QMenu menu(this);
    menu.setStyleSheet("QMenu { background: #1a1a1a; border: 1px solid #444; color: #ddd; } QMenu::item { padding: 6px 20px; } QMenu::item:selected { background: #333; }");
    QAction *useAction = menu.addAction(QStringLiteral("使用 %1").arg(itemName));
    QAction *unequipAction = menu.addAction(QStringLiteral("卸下到背包"));
    QAction *cancelAction = menu.addAction(QStringLiteral("取消"));

    QAction *chosen = menu.exec(QCursor::pos());
    if (chosen == cancelAction || !chosen) return;

    if (chosen == unequipAction) {
        m_player->addItem(itemName);
        m_player->setQuickItem(slot, QString());
        updateQuickItems(m_player->quickItems);
        updateInventory(m_player->inventory);
        return;
    }

    if (chosen == useAction) {
        // Apply item effects
        if (itemName.contains(QStringLiteral("魔力药水")) && !itemName.contains(QStringLiteral("灵药"))) {
            m_player->applyMpDelta(25);
        } else if (itemName.contains(QStringLiteral("药水")) && !itemName.contains(QStringLiteral("灵药"))) {
            m_player->applyHpDelta(30);
        } else if (itemName.contains(QStringLiteral("圣水")) || itemName.contains(QStringLiteral("Holy"))) {
            m_player->applyHpDelta(20); m_player->applyMpDelta(15);
        } else if (itemName.contains(QStringLiteral("灵药")) || itemName.contains(QStringLiteral("Elixir"))) {
            m_player->applyHpDelta(50); m_player->applyMpDelta(30);
        } else {
            m_player->applyHpDelta(15);
        }

        // Consume item
        m_player->removeItem(itemName);
        m_player->setQuickItem(slot, QString());
        updatePlayerStats(m_player->hp, m_player->maxHp, m_player->mp, m_player->maxMp);
        updateQuickItems(m_player->quickItems);
        updateInventory(m_player->inventory);
        emit itemUsed(itemName);
    }
}

void RpgGameWidget::onEquipSlotClicked() {
    QPushButton *btn = qobject_cast<QPushButton*>(sender());
    if (!btn || !m_player) return;

    QString slot = btn->property("eqSlot").toString();
    QString current = m_player->equipmentAt(slot);

    // Unequip → add back to inventory
    if (!current.isEmpty() && current != QStringLiteral("无")) {
        QString prefix = (slot == "weapon") ? QStringLiteral("武器:") :
                         (slot == "armor") ? QStringLiteral("防具:") : QStringLiteral("饰品:");
        m_player->addItem(prefix + current);
        m_player->unequipItem(slot);
        QMessageBox::information(this, QStringLiteral("卸下装备"),
            QStringLiteral("已卸下 %1，放回背包").arg(current));
    } else {
        // Equip from inventory
        QString prefix = (slot == "weapon") ? QStringLiteral("武器:") :
                         (slot == "armor") ? QStringLiteral("防具:") : QStringLiteral("饰品:");
        for (const QString &item : m_player->inventory) {
            if (item.startsWith(prefix)) {
                QString itemName = item.mid(prefix.length());
                m_player->removeItem(item);
                m_player->equipItem(slot, itemName);
                QMessageBox::information(this, QStringLiteral("装备成功"),
                    QStringLiteral("已装备 %1 到 %2 槽位").arg(itemName).arg(
                        slot == "weapon" ? QStringLiteral("武器") :
                        slot == "armor" ? QStringLiteral("防具") : QStringLiteral("饰品")));
                break;
            }
        }
    }

    updateEquipment(m_player->equipment);
    updateInventory(m_player->inventory);
}

void RpgGameWidget::showBackpackDialog() {
    if (!m_player) return;

    QDialog dlg(this);
    dlg.setWindowTitle(QStringLiteral("背包"));
    dlg.setMinimumSize(350, 300);
    dlg.setStyleSheet("QDialog { background: #1a1a1a; border: 2px solid #654; }");

    QVBoxLayout *dlgLayout = new QVBoxLayout(&dlg);

    QLabel *title = new QLabel(QStringLiteral("📦 背包物品"));
    title->setStyleSheet("color: #daa520; font-size: 16px; font-weight: bold; padding: 10px;");
    dlgLayout->addWidget(title);

    if (m_player->inventory.isEmpty()) {
        QLabel *empty = new QLabel(QStringLiteral("（背包为空）"));
        empty->setStyleSheet("color: #555; padding: 20px;");
        dlgLayout->addWidget(empty);
    } else {
        for (int i = 0; i < m_player->inventory.size(); i++) {
            const QString &item = m_player->inventory[i];
            QHBoxLayout *row = new QHBoxLayout();

            QLabel *itemLabel = new QLabel(QStringLiteral("%1. %2").arg(i + 1).arg(item));
            itemLabel->setStyleSheet("color: #ddd; font-size: 12px; padding: 4px;");
            row->addWidget(itemLabel, 1);

            // Quick slot button
            QPushButton *quickBtn = new QPushButton(QStringLiteral("设快捷"));
            quickBtn->setFixedWidth(55);
            quickBtn->setStyleSheet("QPushButton { font-size: 10px; background: #1a2e1a; border: 1px solid #353; color: #8a8; border-radius: 3px; padding: 3px; } QPushButton:hover { background: #224422; }");
            int idx = i;
            connect(quickBtn, &QPushButton::clicked, this, [this, idx, &dlg]() {
                if (!m_player || idx >= m_player->inventory.size()) return;
                for (int s = 0; s < 4; s++) {
                    if (s >= m_player->quickItems.size() || m_player->quickItems[s].isEmpty()) {
                        m_player->setQuickItem(s, m_player->inventory[idx]);
                        updateQuickItems(m_player->quickItems);
                        dlg.accept();
                        return;
                    }
                }
                // All slots full, overwrite slot 0
                m_player->setQuickItem(0, m_player->inventory[idx]);
                updateQuickItems(m_player->quickItems);
                dlg.accept();
            });
            row->addWidget(quickBtn);

            // Equip button for equipment items
            if (item.startsWith(QStringLiteral("武器:")) || item.startsWith(QStringLiteral("防具:")) || item.startsWith(QStringLiteral("饰品:"))) {
                QPushButton *eqBtn = new QPushButton(QStringLiteral("装备"));
                eqBtn->setFixedWidth(45);
                eqBtn->setStyleSheet("QPushButton { font-size: 10px; background: #2e1a1a; border: 1px solid #533; color: #a88; border-radius: 3px; padding: 3px; } QPushButton:hover { background: #442222; }");
                connect(eqBtn, &QPushButton::clicked, this, [this, idx, &dlg]() {
                    if (!m_player || idx >= m_player->inventory.size()) return;
                    QString item = m_player->inventory[idx];
                    QString slot, prefix;
                    if (item.startsWith(QStringLiteral("武器:"))) { slot = "weapon"; prefix = QStringLiteral("武器:"); }
                    else if (item.startsWith(QStringLiteral("防具:"))) { slot = "armor"; prefix = QStringLiteral("防具:"); }
                    else if (item.startsWith(QStringLiteral("饰品:"))) { slot = "accessory"; prefix = QStringLiteral("饰品:"); }
                    else return;
                    QString cur = m_player->equipmentAt(slot);
                    if (!cur.isEmpty() && cur != QStringLiteral("无"))
                        m_player->addItem(prefix + cur);
                    m_player->removeItem(item);
                    m_player->equipItem(slot, item.mid(prefix.length()));
                    updateEquipment(m_player->equipment);
                    updateInventory(m_player->inventory);
                    dlg.accept();
                });
                row->addWidget(eqBtn);
            }

            dlgLayout->addLayout(row);
        }
    }

    QDialogButtonBox *btnBox = new QDialogButtonBox(QDialogButtonBox::Ok);
    btnBox->button(QDialogButtonBox::Ok)->setText(QStringLiteral("关闭"));
    btnBox->setStyleSheet("QPushButton { background: #333; color: #ddd; border: 1px solid #555; padding: 6px 20px; border-radius: 3px; }");
    connect(btnBox, &QDialogButtonBox::accepted, &dlg, &QDialog::accept);
    dlgLayout->addWidget(btnBox);

    dlg.exec();
    updateQuickItems(m_player->quickItems);
    updateInventory(m_player->inventory);
}

void RpgGameWidget::syncEquipFlags() {
    if (!m_player) return;
    // Clear old weapon flags
    QStringList allFlags = {"hero_iron_sword","hero_steel_sword","hero_holy_blade","hero_legend_blade",
        "war_iron_axe","war_battle_axe","war_berserk_axe","war_titan_axe",
        "arc_hunter_bow","arc_long_bow","arc_elf_bow","arc_storm_bow",
        "mag_appr_staff","mag_arcane_staff","mag_elder_staff","mag_arch_staff",
        "pri_oak_mace","pri_blessed_mace","pri_holy_mace","pri_divine_mace"};
    for (const QString &f : allFlags)
        m_player->flags.remove(f);

    // Set current weapon flag
    QString wpn = m_player->equipmentAt("weapon");
    if (wpn.isEmpty() || wpn == QStringLiteral("无")) return;
    QMap<QString,QString> m;
    m["铁剑"]="hero_iron_sword"; m["钢剑"]="hero_steel_sword"; m["圣剑"]="hero_holy_blade"; m["传说之剑"]="hero_legend_blade";
    m["铁拳套"]="war_iron_axe"; m["战拳套"]="war_battle_axe"; m["狂战拳套"]="war_berserk_axe"; m["泰坦拳套"]="war_titan_axe";
    m["猎弓"]="arc_hunter_bow"; m["长弓"]="arc_long_bow"; m["精灵弓"]="arc_elf_bow"; m["风暴弓"]="arc_storm_bow";
    m["学徒法杖"]="mag_appr_staff"; m["奥术法杖"]="mag_arcane_staff"; m["长老法杖"]="mag_elder_staff"; m["大法师杖"]="mag_arch_staff";
    m["圣光之书"]="pri_oak_mace"; m["圣光法典"]="pri_blessed_mace"; m["圣光圣典"]="pri_holy_mace"; m["神圣启示录"]="pri_divine_mace";
    if (m.contains(wpn)) m_player->setFlag(m[wpn]);
}

void RpgGameWidget::getEquipStats(const QString &itemName, int &str, int &agi, int &intel, int &hp, int &mp) {
    str = 0; agi = 0; intel = 0; hp = 0; mp = 0;
    // Weapon stats
    if (itemName == QStringLiteral("铁剑"))        { str = 2; agi = 1; intel = 1; }
    else if (itemName == QStringLiteral("钢剑"))   { str = 3; agi = 2; intel = 2; }
    else if (itemName == QStringLiteral("圣剑"))   { str = 5; agi = 3; intel = 3; hp = 20; }
    else if (itemName == QStringLiteral("传说之剑")) { str = 7; agi = 4; intel = 4; hp = 30; mp = 10; }
    else if (itemName == QStringLiteral("铁拳套"))  { str = 3; agi = 1; }
    else if (itemName == QStringLiteral("战拳套"))  { str = 5; agi = 2; hp = 15; }
    else if (itemName == QStringLiteral("狂战拳套")) { str = 7; agi = 2; hp = 25; }
    else if (itemName == QStringLiteral("泰坦拳套")) { str = 10; agi = 3; hp = 40; }
    else if (itemName == QStringLiteral("猎弓"))    { str = 1; agi = 2; intel = 1; }
    else if (itemName == QStringLiteral("长弓"))    { str = 2; agi = 3; intel = 1; }
    else if (itemName == QStringLiteral("精灵弓"))  { str = 3; agi = 5; intel = 2; hp = 10; }
    else if (itemName == QStringLiteral("风暴弓"))  { str = 4; agi = 7; intel = 3; hp = 15; }
    else if (itemName == QStringLiteral("学徒法杖")) { intel = 3; agi = 1; }
    else if (itemName == QStringLiteral("奥术法杖")) { intel = 5; agi = 1; str = 2; }
    else if (itemName == QStringLiteral("长老法杖")) { intel = 7; agi = 2; str = 2; mp = 20; }
    else if (itemName == QStringLiteral("大法师杖")) { intel = 10; agi = 3; str = 3; mp = 35; }
    else if (itemName == QStringLiteral("圣光之书"))  { intel = 3; mp = 10; }
    else if (itemName == QStringLiteral("圣光法典"))  { intel = 5; mp = 20; hp = 10; }
    else if (itemName == QStringLiteral("圣光圣典"))  { intel = 7; mp = 30; hp = 15; }
    else if (itemName == QStringLiteral("神圣启示录")) { intel = 10; mp = 45; hp = 25; }
    // Armor
    else if (itemName == QStringLiteral("布衣"))     { hp = 10; }
    else if (itemName == QStringLiteral("皮甲"))     { hp = 20; agi = 1; }
    else if (itemName == QStringLiteral("链甲"))     { hp = 30; str = 1; }
    else if (itemName == QStringLiteral("板甲"))     { hp = 50; str = 2; agi = -1; }
    // Accessories
    else if (itemName == QStringLiteral("守护戒指"))  { hp = 15; str = 1; agi = 1; intel = 1; }
    else if (itemName == QStringLiteral("智慧项链"))  { intel = 3; mp = 15; }
    else if (itemName == QStringLiteral("敏捷护符"))  { agi = 3; str = 1; }
    else if (itemName == QStringLiteral("龙鳞挂饰"))  { hp = 30; str = 2; agi = 2; intel = 2; }
}

