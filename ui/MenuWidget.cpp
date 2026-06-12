#include "MenuWidget.h"
#include <QVBoxLayout>
#include <QPushButton>
#include <QLabel>
#include <QFrame>
#include <QApplication>

MenuWidget::MenuWidget(QWidget *parent)
    : QWidget(parent)
{
    setupUi();
}

void MenuWidget::setupUi() {
    setObjectName(QStringLiteral("MenuWidget"));

    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(50, 80, 50, 80);
    mainLayout->setSpacing(25);
    mainLayout->setAlignment(Qt::AlignCenter);

    // 装饰顶部线
    QFrame *topLine = new QFrame(this);
    topLine->setFrameShape(QFrame::HLine);
    topLine->setObjectName(QStringLiteral("menuDecoLine"));
    mainLayout->addWidget(topLine);

    // 游戏大标题
    QLabel *titleLabel = new QLabel(QStringLiteral("钢铁意志：第三帝国的黄昏"), this);
    titleLabel->setObjectName(QStringLiteral("menuTitle"));
    titleLabel->setAlignment(Qt::AlignCenter);
    mainLayout->addWidget(titleLabel);

    // 反战副标题
    QLabel *subTitle = new QLabel(QStringLiteral("—— 一部关于战争残酷与无义的文字悲剧"), this);
    subTitle->setObjectName(QStringLiteral("menuSubtitle"));
    subTitle->setAlignment(Qt::AlignCenter);
    mainLayout->addWidget(subTitle);

    // 装饰中部线
    QFrame *midLine = new QFrame(this);
    midLine->setFrameShape(QFrame::HLine);
    midLine->setObjectName(QStringLiteral("menuDecoLine"));
    mainLayout->addWidget(midLine);

    mainLayout->addSpacing(30);

    // 按钮容器
    QVBoxLayout *btnLayout = new QVBoxLayout();
    btnLayout->setSpacing(15);
    btnLayout->setAlignment(Qt::AlignCenter);

    m_newGameBtn = new QPushButton(QStringLiteral("开始新的战役"), this);
    m_newGameBtn->setObjectName(QStringLiteral("menuBtn"));
    m_newGameBtn->setCursor(Qt::PointingHandCursor);
    m_newGameBtn->setMinimumSize(260, 48);
    connect(m_newGameBtn, &QPushButton::clicked, this, &MenuWidget::newGameClicked);

    m_loadGameBtn = new QPushButton(QStringLiteral("继续旧的战役"), this);
    m_loadGameBtn->setObjectName(QStringLiteral("menuBtn"));
    m_loadGameBtn->setCursor(Qt::PointingHandCursor);
    m_loadGameBtn->setMinimumSize(260, 48);
    connect(m_loadGameBtn, &QPushButton::clicked, this, &MenuWidget::loadGameClicked);

    m_exitGameBtn = new QPushButton(QStringLiteral("退出战役"), this);
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
    QLabel *footer = new QLabel(QStringLiteral("战死沙场者唯有无尽长眠，惟有死者方能见证战争之终结。"), this);
    footer->setObjectName(QStringLiteral("menuFooter"));
    footer->setAlignment(Qt::AlignCenter);
    mainLayout->addWidget(footer);
}
