#include "CharacterCreateWidget.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGridLayout>
#include <QPushButton>
#include <QFrame>
#include <QLabel>
#include <QLineEdit>
#include <QButtonGroup>
#include <QStyle>
#include <QMessageBox>

CharacterCreateWidget::CharacterCreateWidget(QWidget *parent)
    : QWidget(parent)
{
    setupUi();
    onClassSelected(0); // 默认选中步兵
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

    QGridLayout *grid = new QGridLayout();
    grid->setSpacing(10);

    m_classGroup = new QButtonGroup(this);
    m_classGroup->setExclusive(true);

    QStringList classesText = {
        QStringLiteral("国防军步兵\n[步兵]"),
        QStringLiteral("装甲师成员\n[坦克兵]"),
        QStringLiteral("战斗机飞行员\n[空军]"),
        QStringLiteral("轰炸机机组\n[空军]"),
        QStringLiteral("潜艇驾驶员\n[海军]"),
        QStringLiteral("战列舰官兵\n[海军]")
    };

    for (int i = 0; i < 6; ++i) {
        QWidget *card = new QWidget(this);
        card->setObjectName(QStringLiteral("classCard"));
        card->setCursor(Qt::PointingHandCursor);

        QVBoxLayout *cardLayout = new QVBoxLayout(card);
        cardLayout->setContentsMargins(10, 10, 10, 10);
        
        QLabel *cardText = new QLabel(classesText[i], card);
        cardText->setObjectName(QStringLiteral("classCardText"));
        cardText->setAlignment(Qt::AlignCenter);
        cardLayout->addWidget(cardText);

        grid->addWidget(card, i / 2, i % 2);
        m_classCards.append(card);

        // 用透明 QPushButton 作遮罩触发点击
        QPushButton *trigger = new QPushButton(card);
        trigger->setGeometry(0, 0, 180, 80); // resizeEvent 调整，或通过样式表覆层
        trigger->setFlat(true);
        trigger->setStyleSheet(QStringLiteral("background: transparent; border: none;"));
        m_classGroup->addButton(trigger, i);

        connect(trigger, &QPushButton::clicked, this, [this, i]() {
            onClassSelected(i);
        });
    }
    leftLayout->addLayout(grid);
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

void CharacterCreateWidget::onClassSelected(int id) {
    m_selectedClass = static_cast<PlayerClass>(id);

    // 更新左侧选卡高亮
    for (int i = 0; i < 6; ++i) {
        QWidget *card = m_classCards[i];
        card->setProperty("selected", (i == id));
        card->style()->unpolish(card);
        card->style()->polish(card);

        // 调整覆盖层大小
        QPushButton *trigger = qobject_cast<QPushButton*>(m_classGroup->button(i));
        if (trigger) {
            trigger->resize(card->size());
        }
    }

    updatePreview(m_selectedClass);
}

void CharacterCreateWidget::updatePreview(PlayerClass cls) {
    // 根据职业更新属性与背景详情
    switch (cls) {
        case PlayerClass::Infantry:
            m_hpLabel->setText(QStringLiteral("初始生命值：100"));
            m_moraleLabel->setText(QStringLiteral("初始士气值：100"));
            m_scenarioLabel->setText(QStringLiteral("初战方向：【黄色方案】或【斯大林格勒】"));
            m_descLabel->setText(QStringLiteral(
                "国防军步兵。\n\n"
                "你将跟随陆军在泥泞、铁丝网与飞舞的弹片中匍匐前进。你体验着最前线的冰冷雨水与干硬面包，目睹战友的消逝。\n"
                "作为最基础的地面兵种，你在陆地攻坚战（黄色方案、斯大林格勒、柏林）中拥有很强的战术判定优势。但缺乏重甲护卫也意味着直接暴露在炮火之下。"
            ));
            break;
        case PlayerClass::TankCrew:
            m_hpLabel->setText(QStringLiteral("初始生命值：100"));
            m_moraleLabel->setText(QStringLiteral("初始士气值：100"));
            m_scenarioLabel->setText(QStringLiteral("初战方向：【黄色方案】或【斯大林格勒】"));
            m_descLabel->setText(QStringLiteral(
                "装甲师坦克兵。\n\n"
                "你坐在数吨重的钢铁怪兽里，忍受着发动机隆隆轰鸣与满仓刺鼻的汽油烟气。\n"
                "装甲给你提供了对抗枪弹的护盾，让你们能快速突破敌方防线。但在狭窄局促的战车内部，一旦履带受损或遭遇反坦克伏击，逃出着火的铁盒将是你一生中最漫长的几秒钟。"
            ));
            break;
        case PlayerClass::FighterPilot:
            m_hpLabel->setText(QStringLiteral("初始生命值：100"));
            m_moraleLabel->setText(QStringLiteral("初始士气值：100"));
            m_scenarioLabel->setText(QStringLiteral("初战方向：【不列颠空战】"));
            m_descLabel->setText(QStringLiteral(
                "空军战斗机飞行员。\n\n"
                "操纵 Bf 109 战机，在万米高空迎着晨光呼啸前行。高空狗斗是优雅却致命的空中之舞，胜负取决于你和对手在毫秒之间的反应。\n"
                "虽然不用亲历地面堑壕里的污浊与腐尸，但发动机的单薄轰鸣声随时可能因防空火炮或皇家空军的咬尾而中断。你的降落伞是生存的最后寄托。"
            ));
            break;
        case PlayerClass::BomberPilot:
            m_hpLabel->setText(QStringLiteral("初始生命值：100"));
            m_moraleLabel->setText(QStringLiteral("初始士气值：100"));
            m_scenarioLabel->setText(QStringLiteral("初战方向：【不列颠空战】"));
            m_descLabel->setText(QStringLiteral(
                "空军轰炸机飞行员。\n\n"
                "操纵双发 He 111 轰炸机，将高爆航弹投向黑夜之下的城市防线。你必须在防空炮火构成的死亡蛛网和战斗机夹击下保持平稳飞行。\n"
                "每一次掷弹都伴随着大地震颤和道德上的反差——你知道这几百公斤的炸弹砸下去，下方燃烧的街道里将没有赢家。"
            ));
            break;
        case PlayerClass::SubmarineCrew:
            m_hpLabel->setText(QStringLiteral("初始生命值：100"));
            m_moraleLabel->setText(QStringLiteral("初始士气值：100"));
            m_scenarioLabel->setText(QStringLiteral("初战方向：【群狼海战】"));
            m_descLabel->setText(QStringLiteral(
                "海军 U 艇潜艇驾驶员。\n\n"
                "在狭窄、阴暗且散发着机油与汗水恶臭的钢管「铁棺材」中生活。你们在大西洋的暴风雨里静静下潜，扮演狼群撕咬盟军的护航运输线。\n"
                "海面下的航行是与孤独的死斗。而当驱逐舰的声呐回音在耳边以「乒、乒」的声响逼近时，无处可逃的深水炸弹轰击将撕裂每一个人的理智。"
            ));
            break;
        case PlayerClass::BattleshipCrew:
            m_hpLabel->setText(QStringLiteral("初始生命值：100"));
            m_moraleLabel->setText(QStringLiteral("初始士气值：100"));
            m_scenarioLabel->setText(QStringLiteral("初战方向：【群狼海战】"));
            m_descLabel->setText(QStringLiteral(
                "海军主力战列舰水兵。\n\n"
                "站在俾斯麦号或提尔皮茨号宽阔的钢质甲板上，在主炮射击摧枯拉朽的咆哮中领略大洋的宏伟与战舰的荣耀。\n"
                "然而，巨舰大炮的时代已经在航母战机和潜艇突袭中走向没落。一旦这艘移动的钢铁城堡被瘫痪在远海，你和数千名同伴将不得不与这艘巨舰一同沉入无底的深渊。"
            ));
            break;
    }
}

void CharacterCreateWidget::onStartClicked() {
    QString name = m_nameEdit->text().trimmed();
    if (name.isEmpty()) {
        QMessageBox::warning(this, QStringLiteral("警告"), QStringLiteral("为了便于战区联络，军官姓名不能为空。"));
        return;
    }

    emit startGame(name, m_selectedClass);
}
