#include "GameWidget.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QTextBrowser>
#include <QPushButton>
#include <QProgressBar>
#include <QLabel>
#include <QTimer>
#include <QEvent>
#include <QMouseEvent>
#include <QScrollBar>
#include <QStyle>
#include <QDebug>

GameWidget::GameWidget(QWidget *parent)
    : QWidget(parent), m_typeTimer(new QTimer(this))
{
    setupUi();

    m_typeTimer->setInterval(20); // 20ms 打印一个字符
    connect(m_typeTimer, &QTimer::timeout, this, &GameWidget::onTypeTimerTick);

    // 安装事件过滤器以拦截点击跳过打字效果
    m_textBrowser->installEventFilter(this);
    m_textBrowser->viewport()->installEventFilter(this);
}

void GameWidget::setupUi() {
    setObjectName(QStringLiteral("GameWidget"));

    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(30, 25, 30, 25);
    mainLayout->setSpacing(20);

    // ========================================================
    // 1. 顶部状态栏
    // ========================================================
    QWidget *header = new QWidget(this);
    header->setObjectName(QStringLiteral("gameHeader"));
    QHBoxLayout *headerLayout = new QHBoxLayout(header);
    headerLayout->setContentsMargins(15, 10, 15, 10);
    headerLayout->setSpacing(25);

    // 玩家基本信息
    QVBoxLayout *playerInfoLayout = new QVBoxLayout();
    playerInfoLayout->setSpacing(2);
    m_nameLabel = new QLabel(QStringLiteral("军官: 汉斯 · 缪勒"), header);
    m_nameLabel->setObjectName(QStringLiteral("headerNameLabel"));

    m_classLabel = new QLabel(QStringLiteral("兵种: 步兵"), header);
    m_classLabel->setObjectName(QStringLiteral("headerClassLabel"));

    playerInfoLayout->addWidget(m_nameLabel);
    playerInfoLayout->addWidget(m_classLabel);
    headerLayout->addLayout(playerInfoLayout, 2);

    // 属性进度条
    QVBoxLayout *statsLayout = new QVBoxLayout();
    statsLayout->setSpacing(6);

    QHBoxLayout *hpRow = new QHBoxLayout();
    QLabel *hpl = new QLabel(QStringLiteral("生命:"), header);
    hpl->setObjectName(QStringLiteral("headerStatLabel"));
    m_hpBar = new QProgressBar(header);
    m_hpBar->setObjectName(QStringLiteral("gameHpBar"));
    m_hpBar->setRange(0, 100);
    m_hpBar->setValue(100);
    m_hpBar->setTextVisible(true);
    m_hpBar->setFormat(QStringLiteral("%v%"));
    hpRow->addWidget(hpl);
    hpRow->addWidget(m_hpBar);

    QHBoxLayout *morRow = new QHBoxLayout();
    QLabel *morl = new QLabel(QStringLiteral("士气:"), header);
    morl->setObjectName(QStringLiteral("headerStatLabel"));
    m_moraleBar = new QProgressBar(header);
    m_moraleBar->setObjectName(QStringLiteral("gameMoraleBar"));
    m_moraleBar->setRange(0, 100);
    m_moraleBar->setValue(100);
    m_moraleBar->setTextVisible(true);
    m_moraleBar->setFormat(QStringLiteral("%v%"));
    morRow->addWidget(morl);
    morRow->addWidget(m_moraleBar);

    statsLayout->addLayout(hpRow);
    statsLayout->addLayout(morRow);
    headerLayout->addLayout(statsLayout, 3);

    // 场景/关卡信息
    QVBoxLayout *scenLayout = new QVBoxLayout();
    scenLayout->setSpacing(2);
    scenLayout->setAlignment(Qt::AlignRight | Qt::AlignVCenter);

    m_scenarioLabel = new QLabel(QStringLiteral("黄色方案"), header);
    m_scenarioLabel->setObjectName(QStringLiteral("headerScenarioLabel"));
    m_scenarioLabel->setAlignment(Qt::AlignRight);

    m_locationLabel = new QLabel(QStringLiteral("阿登森林"), header);
    m_locationLabel->setObjectName(QStringLiteral("headerLocationLabel"));
    m_locationLabel->setAlignment(Qt::AlignRight);

    scenLayout->addWidget(m_scenarioLabel);
    scenLayout->addWidget(m_locationLabel);
    headerLayout->addLayout(scenLayout, 3);

    mainLayout->addWidget(header);

    // ========================================================
    // 2. 中央文本叙事区
    // ========================================================
    m_textBrowser = new QTextBrowser(this);
    m_textBrowser->setObjectName(QStringLiteral("gameTextBrowser"));
    m_textBrowser->setReadOnly(true);
    m_textBrowser->setFocusPolicy(Qt::NoFocus);
    m_textBrowser->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
    m_textBrowser->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    mainLayout->addWidget(m_textBrowser, 1);

    // ========================================================
    // 3. 底部选项与控制区
    // ========================================================
    QHBoxLayout *bottomLayout = new QHBoxLayout();
    bottomLayout->setSpacing(20);

    // 左侧：选项区域
    m_optionsContainer = new QWidget(this);
    m_optionsContainer->setObjectName(QStringLiteral("optionsContainer"));
    m_optionsLayout = new QVBoxLayout(m_optionsContainer);
    m_optionsLayout->setContentsMargins(0, 0, 0, 0);
    m_optionsLayout->setSpacing(8);
    bottomLayout->addWidget(m_optionsContainer, 3);

    // 右侧：游戏控制（存读档、返回）
    QWidget *ctrlPanel = new QWidget(this);
    ctrlPanel->setObjectName(QStringLiteral("gameCtrlPanel"));
    QVBoxLayout *ctrlLayout = new QVBoxLayout(ctrlPanel);
    ctrlLayout->setContentsMargins(10, 10, 10, 10);
    ctrlLayout->setSpacing(8);

    m_saveBtn = new QPushButton(QStringLiteral("保存战役"), ctrlPanel);
    m_saveBtn->setObjectName(QStringLiteral("gameCtrlBtn"));
    m_saveBtn->setCursor(Qt::PointingHandCursor);
    connect(m_saveBtn, &QPushButton::clicked, this, &GameWidget::saveClicked);

    m_loadBtn = new QPushButton(QStringLiteral("载入战役"), ctrlPanel);
    m_loadBtn->setObjectName(QStringLiteral("gameCtrlBtn"));
    m_loadBtn->setCursor(Qt::PointingHandCursor);
    connect(m_loadBtn, &QPushButton::clicked, this, &GameWidget::loadClicked);

    m_exitBtn = new QPushButton(QStringLiteral("撤回后方"), ctrlPanel);
    m_exitBtn->setObjectName(QStringLiteral("gameCtrlBtn"));
    m_exitBtn->setCursor(Qt::PointingHandCursor);
    connect(m_exitBtn, &QPushButton::clicked, this, &GameWidget::exitClicked);

    ctrlLayout->addWidget(m_saveBtn);
    ctrlLayout->addWidget(m_loadBtn);
    ctrlLayout->addWidget(m_exitBtn);
    bottomLayout->addWidget(ctrlPanel, 1);

    mainLayout->addLayout(bottomLayout);
}

void GameWidget::updatePlayerStats(int hp, int morale) {
    m_hpBar->setValue(hp);
    m_moraleBar->setValue(morale);
}

void GameWidget::updatePlayerInfo(const QString &name, const QString &className) {
    m_nameLabel->setText(QStringLiteral("军官: %1").arg(name));
    m_classLabel->setText(QStringLiteral("兵种: %1").arg(className));
}

void GameWidget::showStoryNode(const StoryNode *node, const QString &playerClassId,
                                const QString &chapterName, bool isLastChapter) {
    if (!node) return;

    m_typeTimer->stop();

    // 更新场景信息
    m_locationLabel->setText(node->locationTitle);
    m_scenarioLabel->setText(chapterName);

    // 获取当前职业的文本（有些节点有职业特定覆盖）
    m_fullText = node->textFor(playerClassId);
    m_currentIndex = 0;
    m_isTyping = true;
    m_textBrowser->clear();

    // 禁用主控按钮，防止打字中途保存
    m_saveBtn->setEnabled(false);
    m_loadBtn->setEnabled(false);

    // 清空当前的选项按钮
    QLayoutItem *child;
    while ((child = m_optionsLayout->takeAt(0)) != nullptr) {
        if (child->widget()) {
            child->widget()->deleteLater();
        }
        delete child;
    }
    m_optionButtons.clear();

    // 根据选项生成按钮，但是暂不显示，直到打字结束
    // 注意: choiceIndex 必须对应 node->choices 的原始下标，
    // 不能使用计数器(因为 classRestricted 选项被跳过会导致错位)
    int origIdx = 0;
    for (const Choice &choice : node->choices) {
        // 职业限制检查
        if (choice.classRestricted && !choice.allowedClasses.contains(playerClassId)) {
            origIdx++;
            continue; // 直接不生成不可选兵种的按钮
        }

        // 标志条件检查 (所需标志不满足则不显示此选项)
        bool flagsMet = true;
        for (const QString &flag : choice.requiredFlags) {
            // 这里无法直接访问PlayerSystem, 暂时不做标志过滤
            // 标志检查由NodeEngine::makeChoice兜底
        }

        QPushButton *btn = new QPushButton(choice.text, m_optionsContainer);
        btn->setObjectName(choice.isCombat ? QStringLiteral("combatOptionBtn") : QStringLiteral("choiceOptionBtn"));
        btn->setCursor(Qt::PointingHandCursor);
        btn->setMinimumHeight(40);
        btn->setVisible(false); // 打字结束前隐藏

        m_optionsLayout->addWidget(btn);
        m_optionButtons.append(btn);

        connect(btn, &QPushButton::clicked, this, &GameWidget::onChoiceButtonClicked);
        // 存储原始数组下标，确保与NodeEngine::makeChoice的索引一致
        btn->setProperty("choiceIndex", origIdx);
        origIdx++;
    }

    // 只有在 NodeType::Narrative 时，提供一个"【继续】"的按钮
    if (node->type == NodeType::Narrative) {
        QPushButton *btn = new QPushButton(QStringLiteral("【继续】"), m_optionsContainer);
        btn->setObjectName(QStringLiteral("choiceOptionBtn"));
        btn->setCursor(Qt::PointingHandCursor);
        btn->setMinimumHeight(40);
        btn->setVisible(false);
        m_optionsLayout->addWidget(btn);
        m_optionButtons.append(btn);

        connect(btn, &QPushButton::clicked, this, &GameWidget::onChoiceButtonClicked);
        btn->setProperty("choiceIndex", 0); // Narrative 选项下 index=0 触发跳转
    }

    // 如果是 Ending 节点，显示特殊跳转按钮以防玩家卡死
    if (node->type == NodeType::Ending || node->isVictory || node->isDefeat) {
        QPushButton *btn = new QPushButton(m_optionsContainer);
        btn->setObjectName(QStringLiteral("choiceOptionBtn"));
        btn->setCursor(Qt::PointingHandCursor);
        btn->setMinimumHeight(40);
        btn->setVisible(false);

        if (node->isVictory) {
            if (isLastChapter) {
                btn->setText(QStringLiteral("【返回主页 — 铭记历史】"));
                connect(btn, &QPushButton::clicked, this, &GameWidget::exitClicked);
            } else {
                btn->setText(QStringLiteral("【进入下一战役】"));
                // 连接到 choiceMade(-2) 表示进入下一场景
                connect(btn, &QPushButton::clicked, this, [this]() {
                    emit choiceMade(-2);
                });
            }
        } else { // 失败结局
            btn->setText(QStringLiteral("【返回主页 — 重新集结】"));
            connect(btn, &QPushButton::clicked, this, &GameWidget::exitClicked);
        }

        m_optionsLayout->addWidget(btn);
        m_optionButtons.append(btn);
    }

    m_typeTimer->start();
}

void GameWidget::onTypeTimerTick() {
    if (m_currentIndex < m_fullText.length()) {
        QTextCursor cursor = m_textBrowser->textCursor();
        cursor.movePosition(QTextCursor::End);
        cursor.insertText(m_fullText.at(m_currentIndex));
        m_currentIndex++;

        // 强制滚动到底部
        m_textBrowser->verticalScrollBar()->setValue(m_textBrowser->verticalScrollBar()->maximum());
    } else {
        displayFullText();
    }
}

void GameWidget::skipTypewriter() {
    if (m_isTyping) {
        displayFullText();
    }
}

void GameWidget::displayFullText() {
    m_typeTimer->stop();
    m_isTyping = false;

    // 渲染全部文本（保证排版）
    m_textBrowser->setPlainText(m_fullText);
    m_textBrowser->verticalScrollBar()->setValue(m_textBrowser->verticalScrollBar()->maximum());

    // 启用选项按钮与存读档按钮
    for (QPushButton *btn : m_optionButtons) {
        btn->setVisible(true);
    }
    m_saveBtn->setEnabled(true);
    m_loadBtn->setEnabled(true);
}

void GameWidget::onChoiceButtonClicked() {
    QPushButton *btn = qobject_cast<QPushButton*>(sender());
    if (!btn) return;

    int idx = btn->property("choiceIndex").toInt();
    emit choiceMade(idx);
}

bool GameWidget::eventFilter(QObject *obj, QEvent *event) {
    if (event->type() == QEvent::MouseButtonPress) {
        QMouseEvent *mouseEvent = static_cast<QMouseEvent*>(event);
        if (mouseEvent->button() == Qt::LeftButton) {
            skipTypewriter();
            return true;
        }
    }
    return QWidget::eventFilter(obj, event);
}
