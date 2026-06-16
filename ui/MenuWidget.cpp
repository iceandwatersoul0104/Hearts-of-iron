/**
 * ===========================================================================
 * MenuWidget.cpp — 主菜单控件实现
 * ===========================================================================
 *
 * 【所属模块】UI 层 — 主菜单
 * 【依赖头文件】MenuWidget.h, Qt Widgets 全家桶 (Layout, Button, Label, Frame, StackedWidget)
 *
 * 本文件实现了游戏的标题画面和 DLC 选择界面。
 *
 * 视觉设计:
 *   主题: "工业战损" 暗色调风格
 *   元素: 顶部装饰线 → 游戏大标题 → 反战副标题 → 中部装饰线 → 按钮组 → 底部标语
 *   装饰线: QFrame::HLine（水平线），通过 QSS 设置颜色
 *   底部标语: "战死沙场者唯有无尽长眠，惟有死者方能见证战争之终结。"
 *
 * DLC 卡片:
 *   动态生成 QPushButton 作为卡片，显示 DLC 标题/副标题/作者。
 *   无效 DLC (valid=false) 显示灰色且不可点击，tooltip 展示错误详情。
 *
 * 布局策略:
 *   使用 Qt::AlignCenter 实现居中布局
 *   使用 QStackedWidget 在主菜单和 DLC 列表之间切换
 * ===========================================================================
 */

#include "MenuWidget.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>
#include <QLabel>
#include <QFrame>
#include <QStackedWidget>
#include <QApplication>

// ===========================================================================
// 构造函数
// ===========================================================================
MenuWidget::MenuWidget(QWidget *parent)
    : QWidget(parent)
{
    setupUi();
}

// ===========================================================================
// setupUi() — 构建主菜单 + DLC 列表双面板
// ===========================================================================
/**
 * 【布局结构】
 *
 *   MenuWidget (最外层)
 *   └── QVBoxLayout (outerLayout)
 *       └── QStackedWidget (内部页面切换)
 *           ├── [0] m_mainMenuPanel
 *           │   ├── 顶部装饰线 (QFrame::HLine)
 *           │   ├── 游戏标题: "钢铁意志：第三帝国的黄昏"
 *           │   ├── 副标题: "—— 一部关于战争残酷与无义的文字悲剧"
 *           │   ├── 中部装饰线
 *           │   ├── 按钮组 (VBoxLayout, 居中)
 *           │   │   ├── "开始新的战役" → newGameClicked()
 *           │   │   ├── "继续旧的战役" → loadGameClicked()
 *           │   │   └── "退出战役"     → exitGameClicked()
 *           │   └── 底部反战标语
 *           │
 *           └── [1] m_dlcListPanel
 *               ├── DLC 页标题: "选择你想体验的DLC"
 *               ├── 副标题: "—— 每个DLC都是一个完整的故事世界"
 *               ├── 装饰线
 *               ├── 动态卡片列表 (m_dlcListLayout)
 *               └── "返回主菜单" 按钮
 */
/*QT图像化界面的使用
1.QVBoxLayout：垂直布局管理器，用于将子控件垂直排列。
2.QHBoxLayout：水平布局管理器，用于将子控件水平排列。
3.QPushButton：按钮控件，用户可以点击触发事件。 
4.QLabel：标签控件，用于显示文本或图像。
5.QFrame：框架控件，可以用来创建装饰线或边框。
6.QStackedWidget：堆叠式窗口控件，可以在多个子控件之间切换显示。
*/
void MenuWidget::setupUi() {
    setObjectName(QStringLiteral("MenuWidget"));

    // ── 最外层布局 ──
    QVBoxLayout *outerLayout = new QVBoxLayout(this);
    outerLayout->setContentsMargins(0, 0, 0, 0);

    // ── 内部页面切换容器 ──
    QStackedWidget *stack = new QStackedWidget(this);

    // ========================================================================
    // Panel 0: 主菜单标题画面
    // ========================================================================
    m_mainMenuPanel = new QWidget(this);
    QVBoxLayout *mainLayout = new QVBoxLayout(m_mainMenuPanel);
    mainLayout->setContentsMargins(50, 80, 50, 80);  // 上下留白
    mainLayout->setSpacing(25);                       // 元素间距
    mainLayout->setAlignment(Qt::AlignCenter);

    // --- 顶部装饰线 ---
    QFrame *topLine = new QFrame(m_mainMenuPanel);
    topLine->setFrameShape(QFrame::HLine);           // 水平线
    topLine->setObjectName(QStringLiteral("menuDecoLine"));  // QSS: 红色细线
    mainLayout->addWidget(topLine);

    // --- 游戏大标题 ---
    QLabel *titleLabel = new QLabel(QStringLiteral("钢铁意志：第三帝国的黄昏"), m_mainMenuPanel);
    titleLabel->setObjectName(QStringLiteral("menuTitle"));  // QSS: 大号字体，居中对齐
    titleLabel->setAlignment(Qt::AlignCenter);
    mainLayout->addWidget(titleLabel);

    // --- 反战副标题 ---
    QLabel *subTitle = new QLabel(QStringLiteral("—— 一部关于战争残酷与无义的文字悲剧"), m_mainMenuPanel);
    subTitle->setObjectName(QStringLiteral("menuSubtitle"));
    subTitle->setAlignment(Qt::AlignCenter);
    mainLayout->addWidget(subTitle);

    // --- 中部装饰线 ---
    QFrame *midLine = new QFrame(m_mainMenuPanel);
    midLine->setFrameShape(QFrame::HLine);
    midLine->setObjectName(QStringLiteral("menuDecoLine"));
    mainLayout->addWidget(midLine);

    mainLayout->addSpacing(30);  // 线与按钮之间的留白

    // --- 按钮组 ---
    QVBoxLayout *btnLayout = new QVBoxLayout();
    btnLayout->setSpacing(15);
    btnLayout->setAlignment(Qt::AlignCenter);

    // 按钮 1: 开始新的战役
    m_newGameBtn = new QPushButton(QStringLiteral("开始新的战役"), m_mainMenuPanel);
    m_newGameBtn->setObjectName(QStringLiteral("menuBtn"));  // QSS: 红色边框按钮
    m_newGameBtn->setCursor(Qt::PointingHandCursor);         // 鼠标悬停变手型
    m_newGameBtn->setMinimumSize(260, 48);                   // 最小尺寸保证可点击
    // 连接信号: 点击 → 发射 newGameClicked() 信号
    connect(m_newGameBtn, &QPushButton::clicked, this, &MenuWidget::newGameClicked);

    // 按钮 2: 继续旧的战役
    m_loadGameBtn = new QPushButton(QStringLiteral("继续旧的战役"), m_mainMenuPanel);
    m_loadGameBtn->setObjectName(QStringLiteral("menuBtn"));
    m_loadGameBtn->setCursor(Qt::PointingHandCursor);
    m_loadGameBtn->setMinimumSize(260, 48);
    // 连接信号: 点击 → 发射 loadGameClicked() 信号
    connect(m_loadGameBtn, &QPushButton::clicked, this, &MenuWidget::loadGameClicked);

    // 按钮 3: 退出战役
    m_exitGameBtn = new QPushButton(QStringLiteral("退出战役"), m_mainMenuPanel);
    m_exitGameBtn->setObjectName(QStringLiteral("menuBtn"));
    m_exitGameBtn->setCursor(Qt::PointingHandCursor);
    m_exitGameBtn->setMinimumSize(260, 48);
    // 连接信号: 点击 → 发射 exitGameClicked() 信号
    connect(m_exitGameBtn, &QPushButton::clicked, this, &MenuWidget::exitGameClicked);

    // 按钮加入布局
    btnLayout->addWidget(m_newGameBtn);
    btnLayout->addWidget(m_loadGameBtn);
    btnLayout->addWidget(m_exitGameBtn);

    mainLayout->addLayout(btnLayout);
    mainLayout->addStretch(1);  // 弹性空间，将按钮组推向上方

    // --- 底部反战标语 ---
    QLabel *footer = new QLabel(QStringLiteral("战死沙场者唯有无尽长眠，惟有死者方能见证战争之终结。"), m_mainMenuPanel);
    footer->setObjectName(QStringLiteral("menuFooter"));  // QSS: 斜体小字
    footer->setAlignment(Qt::AlignCenter);
    mainLayout->addWidget(footer);

    stack->addWidget(m_mainMenuPanel);  // Index 0

    // ========================================================================
    // Panel 1: DLC 选择面板
    // ========================================================================
    m_dlcListPanel = new QWidget(this);
    QVBoxLayout *dlcOuterLayout = new QVBoxLayout(m_dlcListPanel);
    dlcOuterLayout->setContentsMargins(50, 60, 50, 60);
    dlcOuterLayout->setSpacing(20);
    dlcOuterLayout->setAlignment(Qt::AlignCenter);

    // DLC 页标题
    QLabel *dlcTitle = new QLabel(QStringLiteral("选择你想体验的DLC"), m_dlcListPanel);
    dlcTitle->setObjectName(QStringLiteral("menuTitle"));
    dlcTitle->setAlignment(Qt::AlignCenter);
    dlcOuterLayout->addWidget(dlcTitle);

    // DLC 页副标题
    QLabel *dlcSubtitle = new QLabel(QStringLiteral("—— 每个DLC都是一个完整的故事世界"), m_dlcListPanel);
    dlcSubtitle->setObjectName(QStringLiteral("menuSubtitle"));
    dlcSubtitle->setAlignment(Qt::AlignCenter);
    dlcOuterLayout->addWidget(dlcSubtitle);

    // 装饰线
    QFrame *dlcLine = new QFrame(m_dlcListPanel);
    dlcLine->setFrameShape(QFrame::HLine);
    dlcLine->setObjectName(QStringLiteral("menuDecoLine"));
    dlcOuterLayout->addWidget(dlcLine);

    // 动态卡片列表区域 — 每次 showDlcList() 时重建内容
    m_dlcListLayout = new QVBoxLayout();
    m_dlcListLayout->setSpacing(12);
    m_dlcListLayout->setAlignment(Qt::AlignCenter);
    dlcOuterLayout->addLayout(m_dlcListLayout);
    dlcOuterLayout->addStretch(1);  // 将卡片列表推向上方

    // 返回按钮
    m_dlcBackBtn = new QPushButton(QStringLiteral("返回主菜单"), m_dlcListPanel);
    m_dlcBackBtn->setObjectName(QStringLiteral("createBackBtn"));
    m_dlcBackBtn->setCursor(Qt::PointingHandCursor);
    m_dlcBackBtn->setMinimumSize(200, 40);
    // 连接信号: 点击 → 切回主菜单面板
    connect(m_dlcBackBtn, &QPushButton::clicked, this, [this, stack]() {
        stack->setCurrentIndex(0);  // 切回主菜单面板
    });
    dlcOuterLayout->addWidget(m_dlcBackBtn, 0, Qt::AlignCenter);

    stack->addWidget(m_dlcListPanel);  // Index 1

    outerLayout->addWidget(stack);
}

// ===========================================================================
// showDlcList() — 动态生成 DLC 卡片列表
// ===========================================================================
/**
 * 【执行流程】
 *   ① 清空 m_dlcListLayout 中的所有旧卡片（deleteLater 延迟删除）
 *   ② 遍历 manifests 列表:
 *      - 为每个 DLC 创建 QPushButton 卡片
 *      - 卡片显示: 标题 + 副标题 + 作者
 *      - 有效 DLC: 可点击 → emit dlcSelected(dlcId)
 *      - 无效 DLC: 灰色禁用 → tooltip 显示错误列表
 *   ③ 若 manifests 为空 → 显示提示文字
 *   ④ 切换到内部 stacked widget 的 Index 1（DLC 列表面板）
 *
 * 【Lambda 捕获说明】
 *   信号连接使用 [this, dlcId] 按值捕获 dlcId:
 *     dlcId = m.dlcId 是在循环中拷贝的值，不是引用。
 *     这确保信号发射时使用的是正确的 DLC ID。
 */
void MenuWidget::showDlcList(const QList<DlcManifest> &manifests) {
    // ── 清空旧卡片 ──
    QLayoutItem *child;
    while ((child = m_dlcListLayout->takeAt(0)) != nullptr) {
        if (child->widget()) {
            child->widget()->deleteLater();  // 延迟删除（安全擦除正在被事件处理的 widget）
        }
        delete child;  // 删除 layout item 本身
    }

    // ── 生成 DLC 卡片 ──
    if (manifests.isEmpty()) {
        // 空状态: 提示用户放入 DLC
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
            btn->setEnabled(m.valid);  // 无效 DLC 禁用

            // 构建卡片文本
            QString btnText = m.title;
            if (!m.subtitle.isEmpty())
                btnText += "\n" + m.subtitle;
            if (!m.author.isEmpty())
                btnText += QStringLiteral(" — ") + m.author;
            if (!m.valid) {
                btnText += QStringLiteral("\n[⚠ 加载失败，请检查JSON格式]");
                btn->setToolTip(m.errors.join("\n"));  // tooltip 展示所有错误
            }
            btn->setText(btnText);

            // 有效 DLC: 连接点击信号
            if (m.valid) {
                QString dlcId = m.dlcId;  // 拷贝值供 lambda 捕获
                connect(btn, &QPushButton::clicked, this, [this, dlcId]() {
                    emit dlcSelected(dlcId);
                });
            }

            m_dlcListLayout->addWidget(btn);
        }
    }

    // ── 切换到 DLC 列表面板 ──
    QStackedWidget *stack = qobject_cast<QStackedWidget*>(m_dlcListPanel->parentWidget());
    if (stack)
        stack->setCurrentIndex(1);
}
