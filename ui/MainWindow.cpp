#include "MainWindow.h"
#include "MenuWidget.h"
#include "CharacterCreateWidget.h"
#include "GameWidget.h"
#include "SaveLoadDialog.h"
#include "../core/GameEngine.h"
#include "../core/SaveManager.h"
#include "../core/MusicPlayer.h"

#include <QStackedWidget>
#include <QVBoxLayout>
#include <QMessageBox>
#include <QCoreApplication>
#include <QDir>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    m_engine      = new GameEngine(this);
    m_saveManager = new SaveManager(this);
    m_musicPlayer = new MusicPlayer(this);

    setupUi();
    connectSignals();
    initializeMusicTracks();

    // 启动播放主菜单背景音乐
    m_musicPlayer->play(QStringLiteral("main_theme"));
}

void MainWindow::setupUi() {
    setMinimumSize(850, 650);
    resize(960, 700);
    setWindowTitle(QStringLiteral("钢铁意志：第三帝国的黄昏"));

    m_stackedWidget = new QStackedWidget(this);
    setCentralWidget(m_stackedWidget);

    m_menuWidget   = new MenuWidget(this);
    m_createWidget = new CharacterCreateWidget(this);
    m_gameWidget   = new GameWidget(this);

    m_stackedWidget->addWidget(m_menuWidget);       // Index 0
    m_stackedWidget->addWidget(m_createWidget);     // Index 1
    m_stackedWidget->addWidget(m_gameWidget);       // Index 2

    showMainMenu();
}

void MainWindow::connectSignals() {
    // 1. 主菜单信号
    connect(m_menuWidget, &MenuWidget::newGameClicked, this, &MainWindow::showCharacterCreate);
    connect(m_menuWidget, &MenuWidget::loadGameClicked, this, &MainWindow::openLoadDialog);
    connect(m_menuWidget, &MenuWidget::exitGameClicked, this, &MainWindow::close);

    // 2. 角色创建信号
    connect(m_createWidget, &CharacterCreateWidget::backToMenu, this, &MainWindow::showMainMenu);
    connect(m_createWidget, &CharacterCreateWidget::startGame, this, &MainWindow::onStartGame);

    // 3. 游戏界面信号
    connect(m_gameWidget, &GameWidget::choiceMade, this, &MainWindow::onChoiceMade);
    connect(m_gameWidget, &GameWidget::saveClicked, this, &MainWindow::openSaveDialog);
    connect(m_gameWidget, &GameWidget::loadClicked, this, &MainWindow::openLoadDialog);
    connect(m_gameWidget, &GameWidget::exitClicked, this, &MainWindow::showMainMenu);

    // 4. 引擎信号
    connect(m_engine, &GameEngine::nodeChanged, this, &MainWindow::onNodeChanged);
    connect(m_engine, &GameEngine::statsChanged, this, &MainWindow::onStatsChanged);
    connect(m_engine, &GameEngine::combatResult, this, &MainWindow::onCombatResult);
    connect(m_engine, &GameEngine::scenarioVictory, this, &MainWindow::onScenarioVictory);
    connect(m_engine, &GameEngine::scenarioDefeat, this, &MainWindow::onScenarioDefeat);
}

void MainWindow::initializeMusicTracks() {
    // 根据运行路径寻找 music/ 目录，注册各关卡的背景乐
    QString appDir = QCoreApplication::applicationDirPath();
    
    // 支持在开发工作区运行或者在编译构建目录下运行
    QString musicDir = QDir(appDir).filePath(QStringLiteral("music"));
    if (!QDir(musicDir).exists()) {
        // 兼容 CMakeLists 拷贝之前的原始资源路径
        musicDir = QDir(QDir::currentPath()).filePath(QStringLiteral("resources/music"));
    }

    m_musicPlayer->registerTrack(QStringLiteral("main_theme"), QDir(musicDir).filePath(QStringLiteral("main_theme.mp3")));
    m_musicPlayer->registerTrack(QStringLiteral("fallgelb"), QDir(musicDir).filePath(QStringLiteral("fallgelb.mp3")));
    m_musicPlayer->registerTrack(QStringLiteral("britain"), QDir(musicDir).filePath(QStringLiteral("britain.mp3")));
    m_musicPlayer->registerTrack(QStringLiteral("wolfpack"), QDir(musicDir).filePath(QStringLiteral("wolfpack.mp3")));
    m_musicPlayer->registerTrack(QStringLiteral("stalingrad"), QDir(musicDir).filePath(QStringLiteral("stalingrad.mp3")));
    m_musicPlayer->registerTrack(QStringLiteral("stalingrad_winter"), QDir(musicDir).filePath(QStringLiteral("stalingrad_winter.mp3")));
    m_musicPlayer->registerTrack(QStringLiteral("stalingrad_end"), QDir(musicDir).filePath(QStringLiteral("stalingrad_end.mp3")));
    m_musicPlayer->registerTrack(QStringLiteral("berlin"), QDir(musicDir).filePath(QStringLiteral("berlin.mp3")));
    m_musicPlayer->registerTrack(QStringLiteral("berlin_elegy"), QDir(musicDir).filePath(QStringLiteral("berlin_elegy.mp3")));
    m_musicPlayer->registerTrack(QStringLiteral("berlin_end"), QDir(musicDir).filePath(QStringLiteral("berlin_end.mp3")));
    m_musicPlayer->registerTrack(QStringLiteral("defeat_theme"), QDir(musicDir).filePath(QStringLiteral("defeat_theme.mp3")));
}

void MainWindow::showMainMenu() {
    m_stackedWidget->setCurrentWidget(m_menuWidget);
    m_musicPlayer->play(QStringLiteral("main_theme"));
}

void MainWindow::showCharacterCreate() {
    m_stackedWidget->setCurrentWidget(m_createWidget);
}

void MainWindow::showGameScreen() {
    m_stackedWidget->setCurrentWidget(m_gameWidget);
}

void MainWindow::onStartGame(const QString &name, PlayerClass cls) {
    m_engine->newGame(name, cls);

    // 根据职业决定初始战役关卡
    ScenarioId startScen = ScenarioId::FallGelb;
    if (cls == PlayerClass::FighterPilot || cls == PlayerClass::BomberPilot) {
        startScen = ScenarioId::Britain;
    } else if (cls == PlayerClass::SubmarineCrew || cls == PlayerClass::BattleshipCrew) {
        startScen = ScenarioId::WolfPack;
    }

    if (m_engine->startScenario(startScen)) {
        showGameScreen();
        playScenarioMusic(startScen);
    }
}

void MainWindow::onLoadGame(const Player &loadedPlayer) {
    m_engine->loadGame(loadedPlayer);
    showGameScreen();
    playScenarioMusic(m_engine->player().currentScenario);
}

void MainWindow::onChoiceMade(int index) {
    if (index == -2) {
        // 胜利后跳转至下一战役
        int nextId = static_cast<int>(m_engine->player().currentScenario) + 1;
        if (nextId <= static_cast<int>(ScenarioId::Berlin)) {
            ScenarioId nextScen = static_cast<ScenarioId>(nextId);
            if (m_engine->startScenario(nextScen)) {
                playScenarioMusic(nextScen);
            }
        }
    } else {
        // 普通剧情选择
        m_engine->makeChoice(index);
    }
}

void MainWindow::openSaveDialog() {
    Player mutablePlayer = m_engine->player();
    SaveLoadDialog dlg(true, m_saveManager, &mutablePlayer, this);
    dlg.exec();
}

void MainWindow::openLoadDialog() {
    Player mutablePlayer = m_engine->player();
    SaveLoadDialog dlg(false, m_saveManager, &mutablePlayer, this);
    
    // 如果读取成功，触发载入
    connect(&dlg, &SaveLoadDialog::gameLoaded, this, &MainWindow::onLoadGame);
    dlg.exec();
}

void MainWindow::onNodeChanged(const StoryNode *node) {
    if (!node) return;

    // 更新界面节点信息
    m_gameWidget->showStoryNode(node, m_engine->player().playerClass, m_engine->player().currentScenario);
    m_gameWidget->updatePlayerStats(m_engine->player().hp, m_engine->player().morale);
    m_gameWidget->updatePlayerInfo(m_engine->player().name, playerClassName(m_engine->player().playerClass));

    // 根据节点的特定音乐键切换音轨
    if (!node->musicKey.isEmpty()) {
        m_musicPlayer->play(node->musicKey);
    }

    // 剧情变化时自动执行自动存档
    m_saveManager->autoSave(m_engine->player());
}

void MainWindow::onStatsChanged(int hp, int morale) {
    m_gameWidget->updatePlayerStats(hp, morale);
}

void MainWindow::onCombatResult(bool success, int hpChange, int moraleChange) {
    QString title = success ? QStringLiteral("判定成功") : QStringLiteral("判定失败");
    QString msg = success 
        ? QStringLiteral("【判定结果：成功】\n您通过敏捷的动作或精湛的特技成功化险为夷！")
        : QStringLiteral("【判定结果：失败】\n局势恶化！您在交火或事故中遭受了重创。\n生命值损失：%1 | 士气值损失：%2")
          .arg(qAbs(hpChange)).arg(qAbs(moraleChange));

    QMessageBox::information(this, title, msg);
}

void MainWindow::onScenarioVictory(ScenarioId id) {
    // 柏林结局有特定的完结文字，非柏林战役显示关卡胜利弹窗
    if (id != ScenarioId::Berlin) {
        QMessageBox::information(this, QStringLiteral("战役胜利"),
                                 QStringLiteral("【当前战役已顺利通过】\n下一章战役已经解锁。点击选项以继续您的征程。"));
    }
    m_saveManager->autoSave(m_engine->player());
}

void MainWindow::onScenarioDefeat(ScenarioId /*id*/) {
    m_musicPlayer->play(QStringLiteral("defeat_theme"));
    // 失败信息将会在 Ending 节点中展示给玩家
}

void MainWindow::playScenarioMusic(ScenarioId id) {
    switch (id) {
        case ScenarioId::FallGelb:   m_musicPlayer->play(QStringLiteral("fallgelb")); break;
        case ScenarioId::Britain:    m_musicPlayer->play(QStringLiteral("britain")); break;
        case ScenarioId::WolfPack:   m_musicPlayer->play(QStringLiteral("wolfpack")); break;
        case ScenarioId::Stalingrad: m_musicPlayer->play(QStringLiteral("stalingrad")); break;
        case ScenarioId::Berlin:     m_musicPlayer->play(QStringLiteral("berlin")); break;
    }
}
