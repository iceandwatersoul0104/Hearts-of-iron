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

    // Stack to switch between main menu and DLC list
    QStackedWidget *stack = new QStackedWidget(this);

    // ===== Panel 0: Main Menu (KEEP ALL EXISTING CODE EXACTLY) =====
    m_mainMenuPanel = new QWidget(this);
    QVBoxLayout *mainLayout = new QVBoxLayout(m_mainMenuPanel);
    mainLayout->setContentsMargins(50, 80, 50, 80);
    mainLayout->setSpacing(25);
    mainLayout->setAlignment(Qt::AlignCenter);

    // 装饰顶部线
    QFrame *topLine = new QFrame(m_mainMenuPanel);
    topLine->setFrameShape(QFrame::HLine);
    topLine->setObjectName(QStringLiteral("menuDecoLine"));
    mainLayout->addWidget(topLine);

    // 游戏大标题
    QLabel *titleLabel = new QLabel(QStringLiteral("钢铁意志：第三帝国的黄昏"), m_mainMenuPanel);
    titleLabel->setObjectName(QStringLiteral("menuTitle"));
    titleLabel->setAlignment(Qt::AlignCenter);
    mainLayout->addWidget(titleLabel);

    // 反战副标题
    QLabel *subTitle = new QLabel(QStringLiteral("—— 一部关于战争残酷与无义的文字悲剧"), m_mainMenuPanel);
    subTitle->setObjectName(QStringLiteral("menuSubtitle"));
    subTitle->setAlignment(Qt::AlignCenter);
    mainLayout->addWidget(subTitle);

    // 装饰中部线
    QFrame *midLine = new QFrame(m_mainMenuPanel);
    midLine->setFrameShape(QFrame::HLine);
    midLine->setObjectName(QStringLiteral("menuDecoLine"));
    mainLayout->addWidget(midLine);

    mainLayout->addSpacing(30);

    // 按钮容器
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
    mainLayout->addStretch(1);

    // 底部版权/反战标语
    QLabel *footer = new QLabel(QStringLiteral("战死沙场者唯有无尽长眠，惟有死者方能见证战争之终结。"), m_mainMenuPanel);
    footer->setObjectName(QStringLiteral("menuFooter"));
    footer->setAlignment(Qt::AlignCenter);
    mainLayout->addWidget(footer);

    stack->addWidget(m_mainMenuPanel);  // Index 0

    // ===== Panel 1: DLC List =====
    m_dlcListPanel = new QWidget(this);
    QVBoxLayout *dlcOuterLayout = new QVBoxLayout(m_dlcListPanel);
    dlcOuterLayout->setContentsMargins(50, 60, 50, 60);
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

    // Scrollable DLC list area
    m_dlcListLayout = new QVBoxLayout();
    m_dlcListLayout->setSpacing(12);
    m_dlcListLayout->setAlignment(Qt::AlignCenter);
    dlcOuterLayout->addLayout(m_dlcListLayout);
    dlcOuterLayout->addStretch(1);

    m_dlcBackBtn = new QPushButton(QStringLiteral("返回主菜单"), m_dlcListPanel);
    m_dlcBackBtn->setObjectName(QStringLiteral("createBackBtn"));
    m_dlcBackBtn->setCursor(Qt::PointingHandCursor);
    m_dlcBackBtn->setMinimumSize(200, 40);
    connect(m_dlcBackBtn, &QPushButton::clicked, this, [this, stack]() {
        stack->setCurrentIndex(0);
    });
    dlcOuterLayout->addWidget(m_dlcBackBtn, 0, Qt::AlignCenter);

    stack->addWidget(m_dlcListPanel);  // Index 1

    outerLayout->addWidget(stack);
}

void MenuWidget::showDlcList(const QList<DlcManifest> &manifests) {
    // Clear existing DLC cards (except the title/subtitle/back button)
    QLayoutItem *child;
    while ((child = m_dlcListLayout->takeAt(0)) != nullptr) {
        if (child->widget()) {
            child->widget()->deleteLater();
        }
        delete child;
    }

    if (manifests.isEmpty()) {
        QLabel *emptyLabel = new QLabel(QStringLiteral("未检测到任何DLC模块。\n请在dlc/目录下放入DLC文件夹。"), m_dlcListPanel);
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
                btnText += QStringLiteral("\n[⚠ 加载失败，请检查JSON格式]");
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

    // Switch to DLC list view
    QStackedWidget *stack = qobject_cast<QStackedWidget*>(m_dlcListPanel->parentWidget());
    if (stack)
        stack->setCurrentIndex(1);
}
