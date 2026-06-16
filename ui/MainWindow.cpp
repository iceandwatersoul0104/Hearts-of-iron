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

#include <QStackedWidget>
#include <QVBoxLayout>
#include <QMessageBox>
#include <QCoreApplication>
#include <QDir>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    m_dlcManager  = new DlcManager(this);
    m_diceSystem  = new DiceSystem(this);
    m_nodeEngine  = new NodeEngine(m_diceSystem, this);
    m_saveManager = new SaveManager(this);
    m_musicPlayer = new MusicPlayer(this);

    setupUi();
    connectSignals();

    // Scan DLC directory
    QString dlcDir = QCoreApplication::applicationDirPath() + "/dlc";
    if (!QDir(dlcDir).exists())
        dlcDir = QDir::currentPath() + "/dlc";
    m_dlcManager->scanDirectory(dlcDir);

    // 启动后先展示DLC选择列表
    showDlcSelect();
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

    m_stackedWidget->addWidget(m_menuWidget);       // 0
    m_stackedWidget->addWidget(m_createWidget);     // 1
    m_stackedWidget->addWidget(m_gameWidget);       // 2

    showMainMenu();
}

void MainWindow::connectSignals() {
    // Menu
    connect(m_menuWidget, &MenuWidget::newGameClicked, this, &MainWindow::showCharacterCreate);
    connect(m_menuWidget, &MenuWidget::loadGameClicked, this, &MainWindow::openLoadDialog);
    connect(m_menuWidget, &MenuWidget::exitGameClicked, this, &MainWindow::close);
    connect(m_menuWidget, &MenuWidget::dlcSelected, this, &MainWindow::onDlcSelected);

    // Character create
    connect(m_createWidget, &CharacterCreateWidget::backToMenu, this, &MainWindow::showMainMenu);
    connect(m_createWidget, &CharacterCreateWidget::startGame, this, &MainWindow::onStartGame);

    // Game widget
    connect(m_gameWidget, &GameWidget::choiceMade, this, &MainWindow::onChoiceMade);
    connect(m_gameWidget, &GameWidget::saveClicked, this, &MainWindow::openSaveDialog);
    connect(m_gameWidget, &GameWidget::loadClicked, this, &MainWindow::openLoadDialog);
    connect(m_gameWidget, &GameWidget::exitClicked, this, &MainWindow::showMainMenu);

    // Node engine
    connect(m_nodeEngine, &NodeEngine::nodeChanged, this, &MainWindow::onNodeChanged);
    connect(m_nodeEngine, &NodeEngine::statsChanged, this, &MainWindow::onStatsChanged);
    connect(m_nodeEngine, &NodeEngine::combatResult, this, &MainWindow::onCombatResult);
    connect(m_nodeEngine, &NodeEngine::chapterVictory, this, &MainWindow::onChapterVictory);
    connect(m_nodeEngine, &NodeEngine::chapterDefeat, this, &MainWindow::onChapterDefeat);
}

void MainWindow::registerMusicFromDlc() {
    for (auto it = m_currentDlc.music.begin(); it != m_currentDlc.music.end(); ++it) {
        QString fullPath = m_currentDlcBasePath + "/" + it.value();
        m_musicPlayer->registerTrack(it.key(), fullPath);
    }
}

void MainWindow::startChapterMusic() {
    // 章节音乐由 onNodeChanged() 根据节点的 musicKey 自动切换
    // 不再使用 QMap::firstKey() 以避免覆盖正确的章节音乐
}

// --- Navigation ---

void MainWindow::showMainMenu() {
    m_stackedWidget->setCurrentWidget(m_menuWidget);
    m_musicPlayer->play(QStringLiteral("main_theme"));
}

void MainWindow::showDlcSelect() {
    QList<DlcManifest> manifests = m_dlcManager->manifests();
    m_menuWidget->showDlcList(manifests);
    // MenuWidget stays visible, just switches to its internal DLC list view
}

void MainWindow::showCharacterCreate() {
    m_stackedWidget->setCurrentWidget(m_createWidget);
}

void MainWindow::showGameScreen() {
    m_stackedWidget->setCurrentWidget(m_gameWidget);
}

// --- DLC / Game Start ---

void MainWindow::onDlcSelected(const QString &dlcId) {
    const DlcManifest *m = m_dlcManager->getManifest(dlcId);
    if (!m || !m->valid) return;

    m_currentDlc = *m;
    m_currentDlcBasePath = m_dlcManager->dlcBasePath(dlcId);
    registerMusicFromDlc();

    // 将DLC的职业列表传给角色创建界面
    m_createWidget->setClasses(m_currentDlc.classes);

    // 选定DLC后回到主菜单，显示该DLC的标题和副标题
    m_menuWidget->showMainMenu(m_currentDlc.title, m_currentDlc.subtitle);
    m_stackedWidget->setCurrentWidget(m_menuWidget);
    m_musicPlayer->play(QStringLiteral("main_theme"));
}

void MainWindow::onStartGame(const QString &name, const QString &classId) {
    m_player = PlayerSystem(name, classId, m_currentDlc.dlcId);
    m_player.currentChapter = m_currentDlc.startChapter;
    m_player.unlockChapter(m_currentDlc.startChapter);

    // Find class display name
    m_currentClassName = classId;
    for (const auto &c : m_currentDlc.classes) {
        if (c.id == classId) {
            m_currentClassName = c.name;
            break;
        }
    }

    if (m_nodeEngine->startDlc(m_currentDlc, m_currentDlcBasePath, m_player)) {
        showGameScreen();
        startChapterMusic();
    }
}

void MainWindow::onLoadGame(const PlayerSystem &loadedPlayer) {
    m_player = loadedPlayer;

    const DlcManifest *m = m_dlcManager->getManifest(m_player.dlcId);
    if (!m) {
        QMessageBox::warning(this, QStringLiteral("错误"),
                             QStringLiteral("该存档所属的DLC未安装。"));
        return;
    }

    m_currentDlc = *m;
    m_currentDlcBasePath = m_dlcManager->dlcBasePath(m_player.dlcId);
    registerMusicFromDlc();

    m_currentClassName = m_player.classId;
    for (const auto &c : m_currentDlc.classes) {
        if (c.id == m_player.classId) {
            m_currentClassName = c.name;
            break;
        }
    }

    if (m_nodeEngine->startDlc(m_currentDlc, m_currentDlcBasePath, m_player)) {
        showGameScreen();
        startChapterMusic();
    }
}

// --- Choice ---

void MainWindow::onChoiceMade(int index) {
    if (index == -2) {
        // Victory: go to next chapter
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
        m_nodeEngine->makeChoice(index);
    }
}

// --- Save/Load ---

void MainWindow::openSaveDialog() {
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
    dlg.exec();
}

void MainWindow::openLoadDialog() {
    PlayerSystem mutablePlayer;
    SaveLoadDialog dlg(false, m_saveManager, &mutablePlayer, this);
    connect(&dlg, &SaveLoadDialog::gameLoaded, this, &MainWindow::onLoadGame);
    dlg.exec();
}

// --- Engine Signal Handlers ---

void MainWindow::onNodeChanged(const StoryNode *node) {
    if (!node) return;

    // Determine chapter name and whether it's the last chapter
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

    m_gameWidget->showStoryNode(node, m_player.classId, chName, isLastChapter);
    m_gameWidget->updatePlayerStats(m_player.hp, m_player.morale);
    m_gameWidget->updatePlayerInfo(m_player.name, m_currentClassName);

    if (!node->musicKey.isEmpty())
        m_musicPlayer->play(node->musicKey);

    m_saveManager->autoSave(m_player, m_currentDlc.title,
                            m_currentClassName, chName);
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

void MainWindow::onChapterVictory(const QString &chapterId) {
    bool isLastChapter = (chapterId == m_currentDlc.chapters.last().id);
    if (!isLastChapter) {
        QMessageBox::information(this, QStringLiteral("战役胜利"),
                                 QStringLiteral("【当前战役已顺利通过】\n下一章战役已经解锁。"));
    }
    m_saveManager->autoSave(m_player, m_currentDlc.title,
                            m_currentClassName, m_currentChapterName);
}

void MainWindow::onChapterDefeat(const QString & /*chapterId*/) {
    m_musicPlayer->play(QStringLiteral("defeat_theme"));
}
