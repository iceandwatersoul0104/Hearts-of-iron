/**
 * ===========================================================================
 * CharacterCreateWidget.cpp — 角色创建界面实现
 * ===========================================================================
 *
 * 【所属模块】UI 层 — 角色创建
 * 【依赖头文件】CharacterCreateWidget.h, Qt Widgets 全家桶 + QMessageBox, QMouseEvent
 *
 * 本文件实现了角色创建的完整 UI 交互:
 *   - 兵种卡片网格的动态生成（根据 DLC 职业列表）
 *   - 点击卡片的高亮切换（eventFilter 机制）
 *   - 右侧预览面板的联动更新
 *   - 姓名验证 + 开始游戏信号发射
 *
 * 交互亮点:
 *   - 事件过滤器 (eventFilter): 兵种卡片及其所有子控件统一拦截点击，
 *     通过沿父控件链查找 classIndex 属性确定选中的卡片。
 *   - QSS 属性驱动: 通过 setProperty("selected", bool) 切换选中高亮，
 *     然后 unpolish/polish 强制刷新样式。
 * ===========================================================================
 */

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

// ===========================================================================
// 构造函数
// ===========================================================================
CharacterCreateWidget::CharacterCreateWidget(QWidget *parent)
    : QWidget(parent)
{
    setupUi();
}

// ===========================================================================
// setupUi() — 构建角色创建界面的完整布局
// ===========================================================================
/**
 * 【布局结构】
 *
 *   CharacterCreateWidget
 *   ├── 标题: "投身命运 — 创建你的军人"
 *   ├── 姓名输入行 (QHBoxLayout)
 *   │   ├── 标签: "军官姓名："
 *   │   └── 输入框: QLineEdit (默认 "汉斯 · 缪勒", 最长15字符)
 *   │
 *   ├── 核心内容区 (QHBoxLayout, 左右 4:5 比例)
 *   │   ├── 左侧: 兵种选择网格 (QGridLayout, 2列自动换行)
 *   │   │   └── 动态卡片 (由 rebuildClassCards 生成)
 *   │   │
 *   │   └── 右侧: 属性预览卡片 (QWidget#previewCard)
 *   │       ├── "兵种特征简报"
 *   │       ├── HP / 士气 数值
 *   │       ├── 关卡方向
 *   │       ├── 分割线
 *   │       └── 兵种描述文本
 *   │
 *   └── 底部按钮行 (QHBoxLayout)
 *       ├── "返回主菜单" (左)
 *       └── "投身战场"   (右)
 */
void CharacterCreateWidget::setupUi() {
    setObjectName(QStringLiteral("CharacterCreateWidget"));

    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(40, 40, 40, 40);
    mainLayout->setSpacing(25);

    // ============ 标题 ============
    QLabel *titleLabel = new QLabel(QStringLiteral("投身命运 — 创建你的军人"), this);
    titleLabel->setObjectName(QStringLiteral("createTitle"));
    mainLayout->addWidget(titleLabel);

    // ============ 姓名输入区 ============
    QHBoxLayout *nameLayout = new QHBoxLayout();
    nameLayout->setSpacing(15);

    QLabel *nameTip = new QLabel(QStringLiteral("军官姓名："), this);
    nameTip->setObjectName(QStringLiteral("nameTipLabel"));

    m_nameEdit = new QLineEdit(this);
    m_nameEdit->setObjectName(QStringLiteral("nameEdit"));
    m_nameEdit->setText(QStringLiteral("汉斯 · 缪勒"));          // 默认角色名
    m_nameEdit->setMaxLength(15);                                // 限制长度
    m_nameEdit->setPlaceholderText(QStringLiteral("请输入你的姓名..."));

    nameLayout->addWidget(nameTip);
    nameLayout->addWidget(m_nameEdit);
    nameLayout->addStretch();
    mainLayout->addLayout(nameLayout);

    // ============ 核心内容区 (左右分栏 4:5) ============
    QHBoxLayout *contentLayout = new QHBoxLayout();
    contentLayout->setSpacing(30);

    // --- 左侧: 兵种选择网格 ---
    QVBoxLayout *leftLayout = new QVBoxLayout();
    leftLayout->setSpacing(10);

    QLabel *classTip = new QLabel(QStringLiteral("选择你的服役兵种："), this);
    classTip->setObjectName(QStringLiteral("sectionTitle"));
    leftLayout->addWidget(classTip);

    // 网格布局: 2 列自动换行
    m_grid = new QGridLayout();
    m_grid->setSpacing(10);
    leftLayout->addLayout(m_grid);

    contentLayout->addLayout(leftLayout, 4);  // 占 4/9 宽度

    // --- 右侧: 属性预览卡片 ---
    QWidget *previewCard = new QWidget(this);
    previewCard->setObjectName(QStringLiteral("previewCard"));  // QSS: 深色卡片背景

    QVBoxLayout *prevLayout = new QVBoxLayout(previewCard);
    prevLayout->setContentsMargins(20, 20, 20, 20);
    prevLayout->setSpacing(15);

    QLabel *prevTitle = new QLabel(QStringLiteral("兵种特征简报"), previewCard);
    prevTitle->setObjectName(QStringLiteral("previewTitle"));
    prevLayout->addWidget(prevTitle);

    // 属性行: HP + 士气
    QHBoxLayout *statsLayout = new QHBoxLayout();
    m_hpLabel = new QLabel(previewCard);
    m_hpLabel->setObjectName(QStringLiteral("statPreviewLabel"));
    m_moraleLabel = new QLabel(previewCard);
    m_moraleLabel->setObjectName(QStringLiteral("statPreviewLabel"));
    statsLayout->addWidget(m_hpLabel);
    statsLayout->addWidget(m_moraleLabel);
    prevLayout->addLayout(statsLayout);

    // 关卡方向
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

    // 描述文本
    m_descLabel = new QLabel(previewCard);
    m_descLabel->setObjectName(QStringLiteral("previewDescLabel"));
    m_descLabel->setWordWrap(true);                                  // 自动换行
    m_descLabel->setAlignment(Qt::AlignTop | Qt::AlignLeft);        // 左上对齐
    prevLayout->addWidget(m_descLabel, 1);                           // 占剩余空间

    contentLayout->addWidget(previewCard, 5);  // 占 5/9 宽度
    mainLayout->addLayout(contentLayout);

    // ============ 底部控制按钮 ============
    QHBoxLayout *ctrlLayout = new QHBoxLayout();

    m_backBtn = new QPushButton(QStringLiteral("返回主菜单"), this);
    m_backBtn->setObjectName(QStringLiteral("createBackBtn"));
    // 连接信号: 点击 → 切回主菜单面板
    connect(m_backBtn, &QPushButton::clicked, this, &CharacterCreateWidget::backToMenu);

    m_startBtn = new QPushButton(QStringLiteral("投身战场"), this);
    m_startBtn->setObjectName(QStringLiteral("createStartBtn"));  // QSS: 红色强调按钮
    // 连接信号: 点击 → onStartClicked() 进行姓名验证并发射 startGame() 信号
    connect(m_startBtn, &QPushButton::clicked, this, &CharacterCreateWidget::onStartClicked);

    ctrlLayout->addWidget(m_backBtn);
    ctrlLayout->addStretch();  // 将按钮推向两端
    ctrlLayout->addWidget(m_startBtn);
    mainLayout->addLayout(ctrlLayout);
}

// ===========================================================================
// setClasses() — 由 DLC 数据驱动生成兵种卡片
// ===========================================================================
void CharacterCreateWidget::setClasses(const QList<DlcClass> &classes) {
    m_classes = classes;
    m_selectedIndex = 0;           // 默认选中第一个
    rebuildClassCards();           // 重新生成卡片网格
    if (!m_classes.isEmpty()) {
        onClassSelected(0);        // 触发预览更新
    }
}

// ===========================================================================
// rebuildClassCards() — 重建兵种卡片网格
// ===========================================================================
/**
 * 【卡片生成策略】
 *   ① 删除旧卡片（deleteLater 安全释放）
 *   ② 遍历 m_classes，为每个职业创建一个 QWidget 卡片
 *   ③ 卡片按 2 列网格排列: row = i / 2, col = i % 2
 *   ④ 每个卡片设置:
 *      - property("classIndex", i): 用于 eventFilter 识别
 *      - installEventFilter(this): 用于拦截点击
 *   ⑤ 卡片内的子控件也安装事件过滤器（点击 label 也能触发选中）
 */
void CharacterCreateWidget::rebuildClassCards() {
    // ── 清除旧卡片 ──
    for (QWidget *card : m_classCards) {
        m_grid->removeWidget(card);
        card->deleteLater();
    }
    m_classCards.clear();

    // ── 生成新卡片 ──
    for (int i = 0; i < m_classes.size(); ++i) {
        QWidget *card = new QWidget(this);
        card->setObjectName(QStringLiteral("classCard"));        // QSS: 卡片样式
        card->setCursor(Qt::PointingHandCursor);                 // 鼠标悬停变手型
        card->setProperty("classIndex", i);                      // 标记索引
        card->installEventFilter(this);                          // 拦截点击

        QVBoxLayout *cardLayout = new QVBoxLayout(card);
        cardLayout->setContentsMargins(10, 10, 10, 10);

        QLabel *cardText = new QLabel(m_classes[i].name, card);
        cardText->setObjectName(QStringLiteral("classCardText"));
        cardText->setAlignment(Qt::AlignCenter);
        cardLayout->addWidget(cardText);

        // ── 子控件也需安装事件过滤器 ──
        // 因为点击 label 时事件不会传递到 card 上，需要在父链中查找。
        const QList<QObject*> children = card->children();
        for (QObject *child : children) {
            if (child->isWidgetType()) {
                child->installEventFilter(this);
            }
        }

        // 按 2 列网格排列: row = i/2, col = i%2
        m_grid->addWidget(card, i / 2, i % 2);
        m_classCards.append(card);
    }
}

// ===========================================================================
// onClassSelected() — 兵种选中处理
// ===========================================================================
/**
 * 【高亮切换机制】
 *   使用 QSS 属性选择器: QWidget[selected="true"] { border-color: red; }
 *   通过 unpolish/polish 强制 Qt 重新计算样式。
 *
 *   unpolish: 移除旧的样式缓存
 *   polish:   应用新的样式规则
 *   这比 setStyleSheet 更高效，且不会覆盖其他 QSS 规则。
 */
void CharacterCreateWidget::onClassSelected(int id) {
    if (id < 0 || id >= m_classes.size()) return;
    m_selectedIndex = id;

    // ── 更新所有卡片的高亮状态 ──
    for (int i = 0; i < m_classCards.size(); ++i) {
        QWidget *card = m_classCards[i];
        card->setProperty("selected", (i == id));  // 仅选中的卡片设置 selected=true
        card->style()->unpolish(card);             // 清除样式缓存
        card->style()->polish(card);               // 重新应用 QSS
    }

    updatePreview(m_classes[id]);
}

// ===========================================================================
// updatePreview() — 更新右侧预览面板
// ===========================================================================
/**
 * 当前版本显示固定的初始值（HP 100 / 士气 100）。
 * 未来可扩展为根据兵种不同显示不同的初始属性。
 */
void CharacterCreateWidget::updatePreview(const DlcClass &cls) {
    m_hpLabel->setText(QStringLiteral("初始生命值：100"));
    m_moraleLabel->setText(QStringLiteral("初始士气值：100"));
    m_scenarioLabel->setText(QStringLiteral("初战方向：跟随剧情推进"));
    m_descLabel->setText(cls.desc);  // DLC JSON 中定义的职业描述文本
}

// ===========================================================================
// onStartClicked() — "投身战场" 按钮
// ===========================================================================
/**
 * 验证姓名非空 → 发射 startGame 信号。
 * 信号携带:
 *   - name: 角色名（去除首尾空格）
 *   - classId: 选中兵种的字符串 ID
 */
void CharacterCreateWidget::onStartClicked() {
    QString name = m_nameEdit->text().trimmed();
    if (name.isEmpty()) {
        QMessageBox::warning(this, QStringLiteral("警告"),
                             QStringLiteral("为了便于战区联络，军官姓名不能为空。"));
        return;
    }

    emit startGame(name, m_classes[m_selectedIndex].id);
}

// ===========================================================================
// eventFilter() — 事件过滤器（兵种卡片点击检测）
// ===========================================================================
/**
 * 【点击检测算法】
 *   当在任意卡片或其子控件上检测到 MouseButtonPress 事件:
 *     ① 将 obj 转换为 QWidget
 *     ② 沿父控件链向上遍历 (parentWidget())
 *     ③ 查找第一个具有 classIndex 属性的控件
 *     ④ 若找到 → onClassSelected(index) → 返回 true（事件已处理）
 *     ⑤ 若未找到 → 继续传播事件
 *
 * 这种设计的优点:
 *   - 点击卡片上的 label 也能正确识别所属卡片
 *   - 不需要为每个子控件单独连接信号
 */
bool CharacterCreateWidget::eventFilter(QObject *obj, QEvent *event) {
    if (event->type() == QEvent::MouseButtonPress) {
        QWidget *w = qobject_cast<QWidget*>(obj);
        while (w) {
            QVariant idx = w->property("classIndex");
            if (idx.isValid()) {
                onClassSelected(idx.toInt());
                return true;  // 事件已处理，不再传播
            }
            w = w->parentWidget();  // 沿父链向上查找
        }
    }
    return QWidget::eventFilter(obj, event);
}
