#include "MenuWidget.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>
#include <QLabel>
#include <QFrame>
#include <QStackedWidget>
#include <QApplication>

MenuWidget::MenuWidget(QWidget *parent)
    : QWidget(parent)
{
    setupUi();
}

void MenuWidget::setupUi() {
    setObjectName(QStringLiteral("MenuWidget"));

    QVBoxLayout *outerLayout = new QVBoxLayout(this);
    outerLayout->setContentsMargins(0, 0, 0, 0);

    m_stack = new QStackedWidget(this);

    // ===== Panel 0: Category Selection =====
    setupCategoryPanel();

    // ===== Panel 1: Main Menu =====
    m_mainMenuPanel = new QWidget(this);
    QVBoxLayout *mainLayout = new QVBoxLayout(m_mainMenuPanel);
    mainLayout->setContentsMargins(50, 80, 50, 80);
    mainLayout->setSpacing(25);
    mainLayout->setAlignment(Qt::AlignCenter);

    QFrame *topLine = new QFrame(m_mainMenuPanel);
    topLine->setFrameShape(QFrame::HLine);
    topLine->setObjectName(QStringLiteral("menuDecoLine"));
    mainLayout->addWidget(topLine);

    // 游戏标题 (动态更新)
    m_mainTitle = new QLabel(QStringLiteral("钢铁意志：第三帝国的黄昏"), m_mainMenuPanel);
    m_mainTitle->setObjectName(QStringLiteral("menuTitle"));
    m_mainTitle->setAlignment(Qt::AlignCenter);
    mainLayout->addWidget(m_mainTitle);

    // 副标题 (动态更新)
    m_mainSubtitle = new QLabel(QStringLiteral("—— 一部关于战争残酷与无义的文字悲剧"), m_mainMenuPanel);
    m_mainSubtitle->setObjectName(QStringLiteral("menuSubtitle"));
    m_mainSubtitle->setAlignment(Qt::AlignCenter);
    mainLayout->addWidget(m_mainSubtitle);

    QFrame *midLine = new QFrame(m_mainMenuPanel);
    midLine->setFrameShape(QFrame::HLine);
    midLine->setObjectName(QStringLiteral("menuDecoLine"));
    mainLayout->addWidget(midLine);

    mainLayout->addSpacing(30);

    QVBoxLayout *btnLayout = new QVBoxLayout();
    btnLayout->setSpacing(15);
    btnLayout->setAlignment(Qt::AlignCenter);

    m_newGameBtn = new QPushButton(QStringLiteral("开始新的战役"), m_mainMenuPanel);
    m_newGameBtn->setObjectName(QStringLiteral("menuBtn"));
    m_newGameBtn->setCursor(Qt::PointingHandCursor);
    m_newGameBtn->setMinimumSize(260, 48);
    connect(m_newGameBtn, &QPushButton::clicked, this, &MenuWidget::newGameClicked);

    m_loadGameBtn = new QPushButton(QStringLiteral("继续旧的战役"), m_mainMenuPanel);
    m_loadGameBtn->setObjectName(QStringLiteral("menuBtn"));
    m_loadGameBtn->setCursor(Qt::PointingHandCursor);
    m_loadGameBtn->setMinimumSize(260, 48);
    connect(m_loadGameBtn, &QPushButton::clicked, this, &MenuWidget::loadGameClicked);

    m_exitGameBtn = new QPushButton(QStringLiteral("退出战役"), m_mainMenuPanel);
    m_exitGameBtn->setObjectName(QStringLiteral("menuBtn"));
    m_exitGameBtn->setCursor(Qt::PointingHandCursor);
    m_exitGameBtn->setMinimumSize(260, 48);
    connect(m_exitGameBtn, &QPushButton::clicked, this, &MenuWidget::exitGameClicked);

    btnLayout->addWidget(m_newGameBtn);
    btnLayout->addWidget(m_loadGameBtn);
    btnLayout->addWidget(m_exitGameBtn);

    mainLayout->addLayout(btnLayout);

    // 返回DLC选择按钮
    m_dlcBackBtn = new QPushButton(QStringLiteral("返回DLC选择"), m_mainMenuPanel);
    m_dlcBackBtn->setObjectName(QStringLiteral("createBackBtn"));
    m_dlcBackBtn->setCursor(Qt::PointingHandCursor);
    m_dlcBackBtn->setMinimumSize(200, 36);
    connect(m_dlcBackBtn, &QPushButton::clicked, this, [this]() {
        m_stack->setCurrentIndex(2);  // back to DLC list
    });
    mainLayout->addWidget(m_dlcBackBtn, 0, Qt::AlignCenter);

    mainLayout->addStretch(1);

    QLabel *footer = new QLabel(QStringLiteral("战死沙场者唯有无尽长眠，惟有死者方能见证战争之终结。"), m_mainMenuPanel);
    footer->setObjectName(QStringLiteral("menuFooter"));
    footer->setAlignment(Qt::AlignCenter);
    mainLayout->addWidget(footer);

    m_stack->addWidget(m_mainMenuPanel);  // Index 1

    // ===== Panel 2: DLC List =====
    setupDlcListPanel();

    // 默认显示类别选择
    m_stack->setCurrentIndex(0);

    outerLayout->addWidget(m_stack);
}

void MenuWidget::setupCategoryPanel() {
    m_categoryPanel = new QWidget(this);
    QVBoxLayout *catLayout = new QVBoxLayout(m_categoryPanel);
    catLayout->setContentsMargins(50, 80, 50, 80);
    catLayout->setSpacing(30);
    catLayout->setAlignment(Qt::AlignCenter);

    QLabel *catTitle = new QLabel(QStringLiteral("选择你的冒险模式"), m_categoryPanel);
    catTitle->setObjectName(QStringLiteral("menuTitle"));
    catTitle->setAlignment(Qt::AlignCenter);
    catLayout->addWidget(catTitle);

    QLabel *catSubtitle = new QLabel(QStringLiteral("—— 两种截然不同的冒险体验，选择你心仪的征程"), m_categoryPanel);
    catSubtitle->setObjectName(QStringLiteral("menuSubtitle"));
    catSubtitle->setAlignment(Qt::AlignCenter);
    catLayout->addWidget(catSubtitle);

    QFrame *catLine = new QFrame(m_categoryPanel);
    catLine->setFrameShape(QFrame::HLine);
    catLine->setObjectName(QStringLiteral("menuDecoLine"));
    catLayout->addWidget(catLine);

    catLayout->addSpacing(20);

    QHBoxLayout *cardLayout = new QHBoxLayout();
    cardLayout->setSpacing(40);
    cardLayout->setAlignment(Qt::AlignCenter);

    // --- 剧情模式卡片 ---
    QVBoxLayout *narrativeCard = new QVBoxLayout();
    narrativeCard->setSpacing(12);
    narrativeCard->setAlignment(Qt::AlignCenter);

    QPushButton *narrativeBtn = new QPushButton(QStringLiteral("剧情模式"), m_categoryPanel);
    narrativeBtn->setObjectName(QStringLiteral("categoryBtn"));
    narrativeBtn->setCursor(Qt::PointingHandCursor);
    narrativeBtn->setMinimumSize(300, 160);
    narrativeBtn->setStyleSheet(
        "QPushButton#categoryBtn {"
        "  font-size: 28px; font-weight: bold;"
        "  background: #2c3e50; color: #ecf0f1;"
        "  border: 2px solid #5d6d7e; border-radius: 12px;"
        "  padding: 20px;"
        "}"
        "QPushButton#categoryBtn:hover {"
        "  background: #34495e; border-color: #aab7c4;"
        "}"
    );
    connect(narrativeBtn, &QPushButton::clicked, this, [this]() {
        emit categorySelected(QStringLiteral("narrative"));
    });
    narrativeCard->addWidget(narrativeBtn);

    QLabel *narrativeDesc = new QLabel(QStringLiteral("文字叙事 · 侧重剧情与抉择\n无战斗养成 · 纯故事体验"), m_categoryPanel);
    narrativeDesc->setObjectName(QStringLiteral("slotInfo"));
    narrativeDesc->setAlignment(Qt::AlignCenter);
    narrativeDesc->setWordWrap(true);
    narrativeCard->addWidget(narrativeDesc);

    cardLayout->addLayout(narrativeCard);

    // --- 勇者模式卡片 ---
    QVBoxLayout *rpgCard = new QVBoxLayout();
    rpgCard->setSpacing(12);
    rpgCard->setAlignment(Qt::AlignCenter);

    QPushButton *rpgBtn = new QPushButton(QStringLiteral("勇者模式"), m_categoryPanel);
    rpgBtn->setObjectName(QStringLiteral("categoryBtn"));
    rpgBtn->setCursor(Qt::PointingHandCursor);
    rpgBtn->setMinimumSize(300, 160);
    rpgBtn->setStyleSheet(
        "QPushButton#categoryBtn {"
        "  font-size: 28px; font-weight: bold;"
        "  background: #7d3c1e; color: #f5deb3;"
        "  border: 2px solid #b87333; border-radius: 12px;"
        "  padding: 20px;"
        "}"
        "QPushButton#categoryBtn:hover {"
        "  background: #964b2e; border-color: #daa520;"
        "}"
    );
    connect(rpgBtn, &QPushButton::clicked, this, [this]() {
        emit categorySelected(QStringLiteral("rpg"));
    });
    rpgCard->addWidget(rpgBtn);

    QLabel *rpgDesc = new QLabel(QStringLiteral("战斗闯关 · 打怪升级\n装备道具 · 角色养成"), m_categoryPanel);
    rpgDesc->setObjectName(QStringLiteral("slotInfo"));
    rpgDesc->setAlignment(Qt::AlignCenter);
    rpgDesc->setWordWrap(true);
    rpgCard->addWidget(rpgDesc);

    cardLayout->addLayout(rpgCard);

    catLayout->addLayout(cardLayout);
    catLayout->addStretch(1);

    QLabel *catFooter = new QLabel(QStringLiteral("选择模式后，将展示该模式下的所有DLC供你挑选"), m_categoryPanel);
    catFooter->setObjectName(QStringLiteral("menuFooter"));
    catFooter->setAlignment(Qt::AlignCenter);
    catLayout->addWidget(catFooter);

    m_stack->addWidget(m_categoryPanel);  // Index 0
}

void MenuWidget::setupDlcListPanel() {
    m_dlcListPanel = new QWidget(this);
    QVBoxLayout *dlcOuterLayout = new QVBoxLayout(m_dlcListPanel);
    dlcOuterLayout->setContentsMargins(50, 40, 50, 40);
    dlcOuterLayout->setSpacing(20);
    dlcOuterLayout->setAlignment(Qt::AlignCenter);

    QLabel *dlcTitle = new QLabel(QStringLiteral("选择你想体验的DLC"), m_dlcListPanel);
    dlcTitle->setObjectName(QStringLiteral("menuTitle"));
    dlcTitle->setAlignment(Qt::AlignCenter);
    dlcOuterLayout->addWidget(dlcTitle);

    QLabel *dlcSubtitle = new QLabel(QStringLiteral("—— 每个DLC都是一个完整的故事世界"), m_dlcListPanel);
    dlcSubtitle->setObjectName(QStringLiteral("menuSubtitle"));
    dlcSubtitle->setAlignment(Qt::AlignCenter);
    dlcOuterLayout->addWidget(dlcSubtitle);

    QFrame *dlcLine = new QFrame(m_dlcListPanel);
    dlcLine->setFrameShape(QFrame::HLine);
    dlcLine->setObjectName(QStringLiteral("menuDecoLine"));
    dlcOuterLayout->addWidget(dlcLine);

    m_dlcListLayout = new QVBoxLayout();
    m_dlcListLayout->setSpacing(12);
    m_dlcListLayout->setAlignment(Qt::AlignCenter);
    dlcOuterLayout->addLayout(m_dlcListLayout);

    // 返回类别选择按钮
    m_categoryBackBtn = new QPushButton(QStringLiteral("← 返回类别选择"), m_dlcListPanel);
    m_categoryBackBtn->setObjectName(QStringLiteral("createBackBtn"));
    m_categoryBackBtn->setCursor(Qt::PointingHandCursor);
    m_categoryBackBtn->setMinimumSize(200, 36);
    connect(m_categoryBackBtn, &QPushButton::clicked, this, [this]() {
        m_stack->setCurrentIndex(0);
    });
    dlcOuterLayout->addWidget(m_categoryBackBtn, 0, Qt::AlignCenter);

    dlcOuterLayout->addStretch(1);

    m_stack->addWidget(m_dlcListPanel);  // Index 2
}

void MenuWidget::showCategorySelect() {
    m_stack->setCurrentIndex(0);
}

void MenuWidget::showDlcList(const QList<DlcManifest> &manifests) {
    // Clear existing DLC cards (but keep the back button — it's at index 1 in the outer layout, not in m_dlcListLayout)
    // We need to clear only DLC cards. The last item in the layout is stretch, second-to-last is the back button.
    // Let's just remove all items and re-add what we need.
    QLayoutItem *child;
    while ((child = m_dlcListLayout->takeAt(0)) != nullptr) {
        if (child->widget()) child->widget()->deleteLater();
        delete child;
    }

    if (manifests.isEmpty()) {
        QLabel *emptyLabel = new QLabel(QStringLiteral("该模式下暂未检测到DLC模块。\n请在dlc/目录下放入对应类别的DLC文件夹。"), m_dlcListPanel);
        emptyLabel->setObjectName(QStringLiteral("slotInfo"));
        emptyLabel->setAlignment(Qt::AlignCenter);
        m_dlcListLayout->addWidget(emptyLabel);
    } else {
        for (const DlcManifest &m : manifests) {
            QPushButton *btn = new QPushButton(m_dlcListPanel);
            btn->setObjectName(m.valid ? QStringLiteral("menuBtn") : QStringLiteral("menuBtn"));
            btn->setCursor(Qt::PointingHandCursor);
            btn->setMinimumSize(380, 60);
            btn->setEnabled(m.valid);

            QString btnText = m.title;
            if (!m.subtitle.isEmpty())
                btnText += "\n" + m.subtitle;
            if (!m.author.isEmpty())
                btnText += QStringLiteral(" — ") + m.author;
            if (!m.valid) {
                btnText += QStringLiteral("\n[加载失败]");
                btn->setToolTip(m.errors.join("\n"));
            }
            btn->setText(btnText);

            if (m.valid) {
                QString dlcId = m.dlcId;
                connect(btn, &QPushButton::clicked, this, [this, dlcId]() {
                    emit dlcSelected(dlcId);
                });
            }

            m_dlcListLayout->addWidget(btn);
        }
    }

    m_stack->setCurrentIndex(2);
}

void MenuWidget::showMainMenu(const QString &title, const QString &subtitle, const QString &category) {
    m_mainTitle->setText(title);
    m_mainSubtitle->setText(subtitle);

    bool isRpg = (category == QStringLiteral("rpg"));
    m_newGameBtn->setText(isRpg ? QStringLiteral("开始新的冒险") : QStringLiteral("开始新的战役"));
    m_loadGameBtn->setText(isRpg ? QStringLiteral("继续冒险") : QStringLiteral("继续旧的战役"));
    m_exitGameBtn->setText(isRpg ? QStringLiteral("结束冒险") : QStringLiteral("退出战役"));
    m_dlcBackBtn->setText(isRpg ? QStringLiteral("返回DLC选择") : QStringLiteral("返回DLC选择"));

    m_stack->setCurrentIndex(1);
}
