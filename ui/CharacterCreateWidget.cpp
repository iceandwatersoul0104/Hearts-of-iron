#include "CharacterCreateWidget.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGridLayout>
#include <QPushButton>
#include <QFrame>
#include <QLabel>
#include <QLineEdit>
#include <QStyle>
#include <QMessageBox>
#include <QMouseEvent>

CharacterCreateWidget::CharacterCreateWidget(QWidget *parent)
    : QWidget(parent)
{
    setupUi();
}

void CharacterCreateWidget::setupUi() {
    setObjectName(QStringLiteral("CharacterCreateWidget"));

    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(40, 40, 40, 40);
    mainLayout->setSpacing(25);

    // 标题
    QLabel *titleLabel = new QLabel(QStringLiteral("投身命运 — 创建你的军人"), this);
    titleLabel->setObjectName(QStringLiteral("createTitle"));
    mainLayout->addWidget(titleLabel);

    // 姓名输入区
    QHBoxLayout *nameLayout = new QHBoxLayout();
    nameLayout->setSpacing(15);
    QLabel *nameTip = new QLabel(QStringLiteral("军官姓名："), this);
    nameTip->setObjectName(QStringLiteral("nameTipLabel"));

    m_nameEdit = new QLineEdit(this);
    m_nameEdit->setObjectName(QStringLiteral("nameEdit"));
    m_nameEdit->setText(QStringLiteral("汉斯 · 缪勒"));
    m_nameEdit->setMaxLength(15);
    m_nameEdit->setPlaceholderText(QStringLiteral("请输入你的姓名..."));

    nameLayout->addWidget(nameTip);
    nameLayout->addWidget(m_nameEdit);
    nameLayout->addStretch();
    mainLayout->addLayout(nameLayout);

    // 核心内容区 (左侧：职业网格；右侧：详细信息预览)
    QHBoxLayout *contentLayout = new QHBoxLayout();
    contentLayout->setSpacing(30);

    // 左侧：职业选择网格
    QVBoxLayout *leftLayout = new QVBoxLayout();
    leftLayout->setSpacing(10);

    QLabel *classTip = new QLabel(QStringLiteral("选择你的服役兵种："), this);
    classTip->setObjectName(QStringLiteral("sectionTitle"));
    leftLayout->addWidget(classTip);

    m_grid = new QGridLayout();
    m_grid->setSpacing(10);

    leftLayout->addLayout(m_grid);
    contentLayout->addLayout(leftLayout, 4);

    // 右侧：详细预览卡片
    QWidget *previewCard = new QWidget(this);
    previewCard->setObjectName(QStringLiteral("previewCard"));

    QVBoxLayout *prevLayout = new QVBoxLayout(previewCard);
    prevLayout->setContentsMargins(20, 20, 20, 20);
    prevLayout->setSpacing(15);

    QLabel *prevTitle = new QLabel(QStringLiteral("兵种特征简报"), previewCard);
    prevTitle->setObjectName(QStringLiteral("previewTitle"));
    prevLayout->addWidget(prevTitle);

    // 属性行
    QHBoxLayout *statsLayout = new QHBoxLayout();
    m_hpLabel = new QLabel(previewCard);
    m_hpLabel->setObjectName(QStringLiteral("statPreviewLabel"));

    m_moraleLabel = new QLabel(previewCard);
    m_moraleLabel->setObjectName(QStringLiteral("statPreviewLabel"));

    statsLayout->addWidget(m_hpLabel);
    statsLayout->addWidget(m_moraleLabel);
    prevLayout->addLayout(statsLayout);

    // 可选关卡
    m_scenarioLabel = new QLabel(previewCard);
    m_scenarioLabel->setObjectName(QStringLiteral("previewScenarioLabel"));
    m_scenarioLabel->setWordWrap(true);
    prevLayout->addWidget(m_scenarioLabel);

    // 分割线
    QFrame *line = new QFrame(previewCard);
    line->setFrameShape(QFrame::HLine);
    line->setFrameShadow(QFrame::Sunken);
    line->setObjectName(QStringLiteral("previewLine"));
    prevLayout->addWidget(line);

    // 描述文案
    m_descLabel = new QLabel(previewCard);
    m_descLabel->setObjectName(QStringLiteral("previewDescLabel"));
    m_descLabel->setWordWrap(true);
    m_descLabel->setAlignment(Qt::AlignTop | Qt::AlignLeft);
    prevLayout->addWidget(m_descLabel, 1);

    contentLayout->addWidget(previewCard, 5);
    mainLayout->addLayout(contentLayout);

    // 底部控制按钮
    QHBoxLayout *ctrlLayout = new QHBoxLayout();
    m_backBtn = new QPushButton(QStringLiteral("返回主菜单"), this);
    m_backBtn->setObjectName(QStringLiteral("createBackBtn"));
    connect(m_backBtn, &QPushButton::clicked, this, &CharacterCreateWidget::backToMenu);

    m_startBtn = new QPushButton(QStringLiteral("投身战场"), this);
    m_startBtn->setObjectName(QStringLiteral("createStartBtn"));
    connect(m_startBtn, &QPushButton::clicked, this, &CharacterCreateWidget::onStartClicked);

    ctrlLayout->addWidget(m_backBtn);
    ctrlLayout->addStretch();
    ctrlLayout->addWidget(m_startBtn);
    mainLayout->addLayout(ctrlLayout);
}

void CharacterCreateWidget::setClasses(const QList<DlcClass> &classes) {
    m_classes = classes;
    m_selectedIndex = 0;
    rebuildClassCards();
    if (!m_classes.isEmpty()) {
        onClassSelected(0);
    }
}

void CharacterCreateWidget::rebuildClassCards() {
    // Clear existing cards
    for (QWidget *card : m_classCards) {
        m_grid->removeWidget(card);
        card->deleteLater();
    }
    m_classCards.clear();

    // Create dynamic cards from m_classes
    for (int i = 0; i < m_classes.size(); ++i) {
        QWidget *card = new QWidget(this);
        card->setObjectName(QStringLiteral("classCard"));
        card->setCursor(Qt::PointingHandCursor);
        card->setProperty("classIndex", i);
        card->installEventFilter(this);

        QVBoxLayout *cardLayout = new QVBoxLayout(card);
        cardLayout->setContentsMargins(10, 10, 10, 10);

        QLabel *cardText = new QLabel(m_classes[i].name, card);
        cardText->setObjectName(QStringLiteral("classCardText"));
        cardText->setAlignment(Qt::AlignCenter);
        cardLayout->addWidget(cardText);

        // Install event filter on children so clicks on the label propagate
        const QList<QObject*> children = card->children();
        for (QObject *child : children) {
            if (child->isWidgetType()) {
                child->installEventFilter(this);
            }
        }

        m_grid->addWidget(card, i / 2, i % 2);
        m_classCards.append(card);
    }
}

void CharacterCreateWidget::onClassSelected(int id) {
    if (id < 0 || id >= m_classes.size()) return;
    m_selectedIndex = id;

    // Update card selection highlights
    for (int i = 0; i < m_classCards.size(); ++i) {
        QWidget *card = m_classCards[i];
        card->setProperty("selected", (i == id));
        card->style()->unpolish(card);
        card->style()->polish(card);
    }

    updatePreview(m_classes[id]);
}

void CharacterCreateWidget::updatePreview(const DlcClass &cls) {
    m_hpLabel->setText(QStringLiteral("初始生命值：100"));
    m_moraleLabel->setText(QStringLiteral("初始士气值：100"));
    m_scenarioLabel->setText(QStringLiteral("初战方向：跟随剧情推进"));
    m_descLabel->setText(cls.desc);
}

void CharacterCreateWidget::onStartClicked() {
    QString name = m_nameEdit->text().trimmed();
    if (name.isEmpty()) {
        QMessageBox::warning(this, QStringLiteral("警告"), QStringLiteral("为了便于战区联络，军官姓名不能为空。"));
        return;
    }

    emit startGame(name, m_classes[m_selectedIndex].id);
}

bool CharacterCreateWidget::eventFilter(QObject *obj, QEvent *event) {
    if (event->type() == QEvent::MouseButtonPress) {
        // Walk up the parent chain to find the card with classIndex
        QWidget *w = qobject_cast<QWidget*>(obj);
        while (w) {
            QVariant idx = w->property("classIndex");
            if (idx.isValid()) {
                onClassSelected(idx.toInt());
                return true;
            }
            w = w->parentWidget();
        }
    }
    return QWidget::eventFilter(obj, event);
}
