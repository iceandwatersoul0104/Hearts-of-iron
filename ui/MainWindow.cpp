/**
 * ===========================================================================
 * MainWindow.cpp — 应用程序主窗口实现（UI 编排 + 引擎协调）
 * ===========================================================================
 *
 * 【所属模块】UI 层 — 顶层窗口
 * 【依赖头文件】所有 UI 控件头文件 + 所有引擎模块头文件
 *
 * MainWindow 是整个应用程序的"指挥中心"，所有信号/槽的枢纽站。
 *
 * 本文件实现的完整游戏流程:
 *
 *   启动阶段:
 *     构造 → 创建5个引擎实例 → setupUi (3层页面) → connectSignals
 *     → scanDirectory(dlc/) → 播放主菜单音乐
 *
 *   新游戏流程:
 *     点击"开始新的战役" → MenuWidget::newGameClicked
 *     → MainWindow::showDlcSelect → DLC列表显示
 *     → 用户点击DLC → MenuWidget::dlcSelected
 *     → MainWindow::onDlcSelected → 注册音乐 + 设置职业 → CharacterCreateWidget
 *     → 用户选择兵种+输入姓名 → CharacterCreateWidget::startGame
 *     → MainWindow::onStartGame → 创建PlayerSystem → NodeEngine::startDlc
 *     → GameWidget 开始叙事
 *
 *   游戏循环:
 *     节点显示 → 打字机动画 → 选项出现 → 玩家点击
 *     → onChoiceMade → NodeEngine::makeChoice
 *     → nodeChanged 信号 → onNodeChanged
 *     → 更新GameWidget + 切换音乐 + 自动存档
 *     → (重复)
 *
 *   章节推进:
 *     Victory节点 → chapterVictory → onChapterVictory
 *     → 自动存档 + 提示 → 玩家点击"进入下一战役"
 *     → onChoiceMade(-2) → startChapter(下一章) → 新章节开始
 *
 *   读档流程:
 *     点击"继续旧的战役" → MenuWidget::loadGameClicked
 *     → openLoadDialog → 用户选中槽位 → SaveLoadDialog::gameLoaded
 *     → onLoadGame → 恢复DLC信息 + 重新启动引擎 → GameWidget
 * ===========================================================================
 */

#include "MainWindow.h"
#include "MenuWidget.h"
#include "CharacterCreateWidget.h"
#include "GameWidget.h"
#include "SaveLoadDialog.h"
#include "../engine/DlcManager.h"
#include "../engine/NodeEngine.h"
#include "../engine/DiceSystem.h"
#include "../engine/SaveManager.h"
#include "../engine/MusicPlayer.h"

#include <QStackedWidget>             // 页面切换容器
#include <QVBoxLayout>
#include <QMessageBox>                // 战斗结果/提示弹窗
#include <QCoreApplication>           // applicationDirPath (获取 exe 所在目录)
#include <QDir>                       // 目录存在性检查

// ===========================================================================
// 构造函数 — 应用的初始化中枢
// ===========================================================================
/**
 * 【初始化序列】(严格按此顺序执行)
 *
 *   ① 创建 5 个引擎实例:
 *      - DlcManager:   负责扫描和加载 DLC 数据
 *      - DiceSystem:   负责战斗随机判定（注入到 NodeEngine）
 *      - NodeEngine:   负责叙事流程控制（接收 DiceSystem 依赖）
 *      - SaveManager:  负责存档读写（构造时自动创建存档目录）
 *      - MusicPlayer:  负责背景音乐播放（构造时初始化 Qt6 多媒体组件）
 *
 *   ② setupUi():
 *      构建三层 QStackedWidget:
 *        [0] MenuWidget           — 主菜单（标题 + 按钮）
 *        [1] CharacterCreateWidget — 角色创建（兵种 + 姓名）
 *        [2] GameWidget           — 游戏界面（叙事 + 选项 + 状态栏）
 *
 *   ③ connectSignals():
 *      注册所有跨模块信号/槽连接。
 *      这是应用的核心"神经网络"——所有模块通过此处的连接协同工作。
 *
 *   ④ 扫描 DLC 目录:
 *      尝试路径1: <exe所在目录>/dlc (发布版本)
 *      尝试路径2: <当前工作目录>/dlc (开发调试)
 *      DlcManager 遍历子文件夹，解析所有 manifest.json。
 *
 *   ⑤ 播放主菜单音乐:
 *      尝试播放 "main_theme" 键名的音乐。
 *      若未注册（尚无 DLC 选中），静默跳过。
 */
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    // ── 第 1 步: 创建引擎实例 ──
    // 所有引擎实例以 this 为父对象，Qt 自动管理生命周期
    
    //dlcManager 负责扫描和加载 DLC 数据
    m_dlcManager  = new DlcManager(this);
    //diceSystem 负责战斗随机判定（注入到 NodeEngine）
    m_diceSystem  = new DiceSystem(this);
    //nodeEngine 负责叙事流程控制（接收 DiceSystem 依赖）
    m_nodeEngine  = new NodeEngine(m_diceSystem, this);  // 依赖注入: DiceSystem → NodeEngine
    //saveManager 负责存档读写（构造时自动创建存档目录）
    m_saveManager = new SaveManager(this);               // 构造时自动创建存档目录
    //musicPlayer 负责背景音乐播放（构造时初始化 Qt6 多媒体组件）
    m_musicPlayer = new MusicPlayer(this);               // 构造时初始化 Qt6 多媒体

    // ── 第 2~3 步: 构建 UI 并连接信号 ──
    setupUi();
    connectSignals();

    // ── 第 4 步: 扫描 DLC 目录 ──
    // 优先使用 exe 所在目录（发布版本），若不存在则回退到当前工作目录（开发调试）
    QString dlcDir = QCoreApplication::applicationDirPath() + "/dlc";
    if (!QDir(dlcDir).exists())
        dlcDir = QDir::currentPath() + "/dlc";
    m_dlcManager->scanDirectory(dlcDir);

    // ── 第 5 步: 播放主菜单主题音乐 ──
    // 此时音乐可能尚未注册（DLC 选择后才注册），静默跳过。
    m_musicPlayer->play(QStringLiteral("main_theme"));
}

// ===========================================================================
// setupUi() — 构建三层页面结构
// ===========================================================================
/**
 * 【页面布局】
 *
 *   QMainWindow
 *   └── QStackedWidget (centralWidget)
 *       ├── [0] MenuWidget           — 主菜单 + DLC 列表内嵌
 *       ├── [1] CharacterCreateWidget — 兵种选择 + 姓名输入
 *       └── [2] GameWidget           — 核心游戏界面
 *
 * 所有页面在构造时创建，通过 setCurrentWidget 切换。
 * 不会动态销毁/重建页面（保持状态简单）。
 *
 * 窗口尺寸:
 *   最小: 850×650 (确保选项按钮不重叠)
 *   默认: 960×700
 */
void MainWindow::setupUi() {
    setMinimumSize(850, 650);
    resize(960, 700);
    setWindowTitle(QStringLiteral("钢铁意志：第三帝国的黄昏"));

    // ── 创建 QStackedWidget 作为中央控件 ──
    m_stackedWidget = new QStackedWidget(this);
    setCentralWidget(m_stackedWidget);

    // ── 创建三个页面 ──
    m_menuWidget   = new MenuWidget(this);
    m_createWidget = new CharacterCreateWidget(this);
    m_gameWidget   = new GameWidget(this);

    // ── 将页面加入堆栈（索引即页面编号） ──
    m_stackedWidget->addWidget(m_menuWidget);       // Index 0: 主菜单
    m_stackedWidget->addWidget(m_createWidget);     // Index 1: 角色创建
    m_stackedWidget->addWidget(m_gameWidget);       // Index 2: 游戏界面

    // ── 默认显示主菜单 ──
    showMainMenu();
}

// ===========================================================================
// connectSignals() — 应用的核心"神经网络"
// ===========================================================================
/**
 * 【信号连接总览】
 *
 *   MenuWidget ────────→ MainWindow ────────→ 引擎
 *     创建新游戏 加载DLC
 *     newGameClicked  →  showDlcSelect
 *     游戏加载
 *     loadGameClicked →  openLoadDialog
 *     退出游戏
 *     exitGameClicked →  close
 *     加载DLC
 *     dlcSelected     →  onDlcSelected
 *
 *     1.返回主菜单-》切换页面+音乐 2.加载DLC-》注册音乐+设置职业列表
 *   CharacterCreateWidget → MainWindow
 *     backToMenu  →  showMainMenu
 *     startGame   →  onStartGame
 *
 *   GameWidget ────────→ MainWindow ────────→ NodeEngine
 *     choiceMade  →  onChoiceMade       →  makeChoice
 *     saveClicked →  openSaveDialog
 *     loadClicked →  openLoadDialog
 *     exitClicked →  showMainMenu
 *
 *   NodeEngine ────────→ MainWindow ────────→ GameWidget
 *     nodeChanged      →  onNodeChanged    →  showStoryNode
 *     statsChanged     →  onStatsChanged   →  updatePlayerStats
 *     combatResult     →  onCombatResult   →  QMessageBox
 *     chapterVictory   →  onChapterVictory →  自动存档
 *     chapterDefeat    →  onChapterDefeat  →  失败音乐
 */
void MainWindow::connectSignals() {
    // ============ Menu 信号 ============
    connect(m_menuWidget, &MenuWidget::newGameClicked,
            this, &MainWindow::showDlcSelect);
    connect(m_menuWidget, &MenuWidget::loadGameClicked,
            this, &MainWindow::openLoadDialog);
    connect(m_menuWidget, &MenuWidget::exitGameClicked,
            this, &MainWindow::close);
    connect(m_menuWidget, &MenuWidget::dlcSelected,
            this, &MainWindow::onDlcSelected);

    // ============ Character Create 信号 ============
    connect(m_createWidget, &CharacterCreateWidget::backToMenu,
            this, &MainWindow::showMainMenu);
    connect(m_createWidget, &CharacterCreateWidget::startGame,
            this, &MainWindow::onStartGame);

    // ============ Game Widget 信号 ============
    connect(m_gameWidget, &GameWidget::choiceMade,
            this, &MainWindow::onChoiceMade);
    connect(m_gameWidget, &GameWidget::saveClicked,
            this, &MainWindow::openSaveDialog);
    connect(m_gameWidget, &GameWidget::loadClicked,
            this, &MainWindow::openLoadDialog);
    connect(m_gameWidget, &GameWidget::exitClicked,
            this, &MainWindow::showMainMenu);

    // ============ Node Engine 信号 ============
    connect(m_nodeEngine, &NodeEngine::nodeChanged,
            this, &MainWindow::onNodeChanged);
    connect(m_nodeEngine, &NodeEngine::statsChanged,
            this, &MainWindow::onStatsChanged);
    connect(m_nodeEngine, &NodeEngine::combatResult,
            this, &MainWindow::onCombatResult);
    connect(m_nodeEngine, &NodeEngine::chapterVictory,
            this, &MainWindow::onChapterVictory);
    connect(m_nodeEngine, &NodeEngine::chapterDefeat,
            this, &MainWindow::onChapterDefeat);
}

// ===========================================================================
// registerMusicFromDlc() — 注册 DLC 中所有音乐
// ===========================================================================
/**
 * 遍历 m_currentDlc.music 映射（音乐键 → 相对路径），
 * 拼接 DLC 基础路径得到绝对路径，然后注册到 MusicPlayer。
 *
 * 示例:
 *   music: { "main_theme": "music/main.mp3", "battle": "music/battle.mp3" }
 *   basePath: "C:/path/to/dlc/third_reich"
 *   → registerTrack("main_theme", "C:/path/to/dlc/third_reich/music/main.mp3")
 */
void MainWindow::registerMusicFromDlc() {
    for (auto it = m_currentDlc.music.begin(); it != m_currentDlc.music.end(); ++it) {
        QString fullPath = m_currentDlcBasePath + "/" + it.value();
        m_musicPlayer->registerTrack(it.key(), fullPath);
    }
}

// ===========================================================================
// startChapterMusic() — 播放章节的第一首音乐
// ===========================================================================
/**
 * 如果 DLC 注册了任何音乐，则播放映射表中的第一首。
 * 之后当玩家进入指定了 musicKey 的节点时，音乐会按节点切换。
 */
void MainWindow::startChapterMusic() {
    if (!m_currentDlc.music.isEmpty()) {
        QString firstKey = m_currentDlc.music.firstKey();
        m_musicPlayer->play(firstKey);
    }
}

// ===========================================================================
// 页面导航槽 — 切换 QStackedWidget 的活动页面
// ===========================================================================
void MainWindow::showMainMenu() {
    m_stackedWidget->setCurrentWidget(m_menuWidget);
    m_musicPlayer->play(QStringLiteral("main_theme"));  // 切回主菜单音乐
}

//加载DLC页面，显示DLC列表（由 MenuWidget 内部的 stacked widget 切换实现）
void MainWindow::showDlcSelect() {
    QList<DlcManifest> manifests = m_dlcManager->manifests();
    m_menuWidget->showDlcList(manifests);
    // MenuWidget 保持在当前页面（index 0），仅切换其内部的 stacked widget
}

void MainWindow::showCharacterCreate() {
    m_stackedWidget->setCurrentWidget(m_createWidget);
}

void MainWindow::showGameScreen() {
    m_stackedWidget->setCurrentWidget(m_gameWidget);
}

// ===========================================================================
// onDlcSelected() — DLC 选择处理
// ===========================================================================
/**
 * 【执行流程】
 *   ① 从 DlcManager 获取清单（包含职业列表、章节列表、音乐映射）
 *   ② 存储当前 DLC 清单和路径
 *   ③ 注册 DLC 中所有音乐文件到 MusicPlayer
 *   ④ 将职业列表传递给 CharacterCreateWidget（驱动物理兵种卡片生成）
 *   ⑤ 切换到角色创建页面
 *
 * 【无效 DLC 处理】
 *   若 DLC 验证未通过（valid=false），直接返回不处理。
 *   MenuWidget 已通过 setEnabled(false) 禁用无效 DLC 的按钮。
 */
void MainWindow::onDlcSelected(const QString &dlcId) {
    const DlcManifest *m = m_dlcManager->getManifest(dlcId);
    if (!m || !m->valid) return;  // 无效 DLC → 忽略

    m_currentDlc = *m;
    m_currentDlcBasePath = m_dlcManager->dlcBasePath(dlcId);
    registerMusicFromDlc();                     // 批量注册音乐

    m_createWidget->setClasses(m_currentDlc.classes);  // 驱动 UI 生成职业卡片
    showCharacterCreate();
}

// ===========================================================================
// onStartGame() — 新游戏启动
// ===========================================================================
/**
 * 【执行流程】
 *   ① 创建 PlayerSystem(name, classId, dlcId)
 *      - HP/士气初始 100
 *      - 标志/进度为空
 *   ② 设置初始章节 = manifest.startChapter
 *   ③ 解锁初始章节
 *   ④ 解析兵种显示名称（m_currentClassName）
 *   ⑤ 调用 NodeEngine::startDlc() 启动引擎
 *   ⑥ 切换到游戏界面 + 播放章节音乐
 */
void MainWindow::onStartGame(const QString &name, const QString &classId) {
    // ── 创建玩家状态 ──
    m_player = PlayerSystem(name, classId, m_currentDlc.dlcId);
    m_player.currentChapter = m_currentDlc.startChapter;
    m_player.unlockChapter(m_currentDlc.startChapter);

    // ── 查找兵种显示名称 ──
    m_currentClassName = classId;  // 默认显示 ID（兜底）
    for (const auto &c : m_currentDlc.classes) {
        if (c.id == classId) {
            m_currentClassName = c.name;
            break;
        }
    }

    // ── 启动引擎 ──
    if (m_nodeEngine->startDlc(m_currentDlc, m_currentDlcBasePath, m_player)) {
        showGameScreen();
        startChapterMusic();
    }
}

// ===========================================================================
// onLoadGame() — 存档读取处理
// ===========================================================================
/**
 * 【执行流程】
 *   ① 从 SaveLoadDialog 接收恢复后的 PlayerSystem
 *   ② 根据 player.dlcId 查找对应的 DLC 清单
 *      - 若 DLC 未安装（如删除了 dlc/ 文件夹）→ 警告并返回
 *   ③ 恢复 DLC 上下文（清单、路径、兵种名称）
 *   ④ 注册音乐
 *   ⑤ 调用 NodeEngine::startDlc() → 从存档中的 currentChapter 继续
 *      （startDlc 内部检查 player.currentChapter 是否为空，
 *        非空时跳过起始章节直接使用当前章节）
 *   ⑥ 切换到游戏界面
 */
void MainWindow::onLoadGame(const PlayerSystem &loadedPlayer) {
    m_player = loadedPlayer;

    // ── 查找存档对应的 DLC ──
    const DlcManifest *m = m_dlcManager->getManifest(m_player.dlcId);
    if (!m) {
        QMessageBox::warning(this, QStringLiteral("错误"),
                             QStringLiteral("该存档所属的DLC未安装。"));
        return;
    }

    // ── 恢复 DLC 上下文 ──
    m_currentDlc = *m;
    m_currentDlcBasePath = m_dlcManager->dlcBasePath(m_player.dlcId);
    registerMusicFromDlc();

    // ── 恢复兵种名称 ──
    m_currentClassName = m_player.classId;
    for (const auto &c : m_currentDlc.classes) {
        if (c.id == m_player.classId) {
            m_currentClassName = c.name;
            break;
        }
    }

    // ── 重新启动引擎（从存档章节继续） ──
    if (m_nodeEngine->startDlc(m_currentDlc, m_currentDlcBasePath, m_player)) {
        showGameScreen();
        startChapterMusic();
    }
}

// ===========================================================================
// onChoiceMade() — 玩家选择处理
// ===========================================================================
/**
 * 【分支处理】
 *
 *   index = -2: （特殊值）胜利后进入下一章节
 *     在 manifest.chapters 中查找当前章节的下一章，
 *     调用 NodeEngine::startChapter(nextChId) 开始新章节。
 *
 *   index = -1: Narrative 节点的"继续"
 *     调用 NodeEngine::makeChoice(-1) → 内部处理为 NavigateTo(nextNodeId)
 *
 *   index >= 0: 普通选项或战斗选项
 *     调用 NodeEngine::makeChoice(index) → 执行完整的选项逻辑
 */
void MainWindow::onChoiceMade(int index) {
    if (index == -2) {
        // ── 胜利后进入下一章节（特殊值 -2） ──
        const QList<DlcChapterMeta> &chapters = m_currentDlc.chapters;
        QString currentCh = m_player.currentChapter;
        for (int i = 0; i < chapters.size(); ++i) {
            if (chapters[i].id == currentCh && i + 1 < chapters.size()) {
                QString nextChId = chapters[i + 1].id;
                if (m_nodeEngine->startChapter(nextChId)) {
                    startChapterMusic();
                }
                break;
            }
        }
    } else {
        // ── 普通/战斗选项 → 委托给 NodeEngine ──
        m_nodeEngine->makeChoice(index);
    }
}

// ===========================================================================
// openSaveDialog() / openLoadDialog() — 存档对话框
// ===========================================================================
/**
 * 【保存对话框】
 *   创建 SaveLoadDialog(true, ...) → savingMode=true
 *   → 显示 4 个槽位 → 用户选择 → 保存确认 → 写入 JSON
 *
 *   注意: 传入 m_player 的可变引用，但保存操作只在用户确认后才执行。
 *   setDlcInfo 提供存档摘要所需的显示信息。
 *
 * 【读取对话框】
 *   创建 SaveLoadDialog(false, ...) → savingMode=false
 *   → 显示 4 个槽位 → 用户选择 → gameLoaded 信号 → onLoadGame
 */
void MainWindow::openSaveDialog() {
    // 查找当前章节的显示名称
    QString chName;
    for (const auto &ch : m_currentDlc.chapters) {
        if (ch.id == m_player.currentChapter) {
            chName = ch.name;
            break;
        }
    }

    PlayerSystem mutablePlayer = m_player;
    SaveLoadDialog dlg(true, m_saveManager, &mutablePlayer, this);
    dlg.setDlcInfo(m_currentDlc.title, m_currentClassName, chName);
    dlg.exec();  // 模态对话框，阻塞直到关闭
}

void MainWindow::openLoadDialog() {
    PlayerSystem mutablePlayer;
    SaveLoadDialog dlg(false, m_saveManager, &mutablePlayer, this);
    connect(&dlg, &SaveLoadDialog::gameLoaded, this, &MainWindow::onLoadGame);
    dlg.exec();
}

// ===========================================================================
// onNodeChanged() — 节点切换响应 (关键槽)
// ===========================================================================
/**
 * 这是连接引擎和 UI 的核心桥接函数，每次节点切换都会触发。
 *
 * 【执行流程】
 *   ① 查找当前章节的显示名称 + 判断是否为最后一章
 *   ② 更新 GameWidget:
 *      - showStoryNode(): 显示新的叙事文本 + 生成选项按钮
 *      - updatePlayerStats(): 刷新 HP/士气进度条
 *      - updatePlayerInfo(): 刷新玩家姓名/兵种
 *   ③ 若节点指定了 musicKey → 切换背景音乐
 *   ④ 自动存档 → SaveManager::autoSave() 写入 slot 0
 *
 * 【最后一章判断】
 *   用于 GameWidget 决定胜利按钮文案:
 *     - 非最后一章: "【进入下一战役】"
 *     - 最后一章:   "【返回主页 — 铭记历史】"
 */
void MainWindow::onNodeChanged(const StoryNode *node) {
    if (!node) return;

    // ── 获取当前章节信息 ──
    QString chName;
    bool isLastChapter = false;
    for (const auto &ch : m_currentDlc.chapters) {
        if (ch.id == m_player.currentChapter) {
            chName = ch.name;
            isLastChapter = (ch.id == m_currentDlc.chapters.last().id);
            break;
        }
    }

    m_currentChapterName = chName;

    // ── 更新 GameWidget 显示 ──
    m_gameWidget->showStoryNode(node, m_player.classId, chName, isLastChapter);
    m_gameWidget->updatePlayerStats(m_player.hp, m_player.morale);
    m_gameWidget->updatePlayerInfo(m_player.name, m_currentClassName);

    // ── 切换音乐（若节点指定） ──
    if (!node->musicKey.isEmpty())
        m_musicPlayer->play(node->musicKey);

    // ── 自动存档 ──
    m_saveManager->autoSave(m_player, m_currentDlc.title,
                            m_currentClassName, chName);
}

// ===========================================================================
// onStatsChanged() — 属性变化响应
// ===========================================================================
void MainWindow::onStatsChanged(int hp, int morale) {
    m_gameWidget->updatePlayerStats(hp, morale);
}

// ===========================================================================
// onCombatResult() — 战斗结果响应
// ===========================================================================
/**
 * 弹出模态消息框显示战斗判定结果。
 * - 成功: 简短祝贺文本
 * - 失败: 显示 HP/士气损失详情
 *
 * 使用 qAbs() 取绝对值，因为数值已为负数（如 -20）。
 */
void MainWindow::onCombatResult(bool success, int hpChange, int moraleChange) {
    QString title = success ? QStringLiteral("判定成功") : QStringLiteral("判定失败");
    QString msg = success
        ? QStringLiteral("【判定结果：成功】\n您通过敏捷的动作或精湛的特技成功化险为夷！")
        : QStringLiteral("【判定结果：失败】\n局势恶化！您在交火或事故中遭受了重创。\n生命值损失：%1 | 士气值损失：%2")
          .arg(qAbs(hpChange)).arg(qAbs(moraleChange));
    QMessageBox::information(this, title, msg);
}

// ===========================================================================
// onChapterVictory() — 章节胜利响应
// ===========================================================================
/**
 * 非最后一章: 弹出提示告知下一章已解锁
 * 最后一章:   不弹窗（GameWidget 会显示特殊按钮）
 * 自动存档:   始终执行，保存当前状态
 */
void MainWindow::onChapterVictory(const QString &chapterId) {
    bool isLastChapter = (chapterId == m_currentDlc.chapters.last().id);
    if (!isLastChapter) {
        QMessageBox::information(this, QStringLiteral("战役胜利"),
                                 QStringLiteral("【当前战役已顺利通过】\n下一章战役已经解锁。"));
    }
    m_saveManager->autoSave(m_player, m_currentDlc.title,
                            m_currentClassName, m_currentChapterName);
}

// ===========================================================================
// onChapterDefeat() — 章节失败响应
// ===========================================================================
/**
 * 播放失败主题音乐，营造悲剧氛围。
 * 不弹窗（由 GameWidget 显示失败按钮）。
 */
void MainWindow::onChapterDefeat(const QString & /*chapterId*/) {
    m_musicPlayer->play(QStringLiteral("defeat_theme"));
}
