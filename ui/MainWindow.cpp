#include "MainWindow.h"
#include "MenuWidget.h"
#include "CharacterCreateWidget.h"
#include "GameWidget.h"
#include "RpgGameWidget.h"
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
#include <QRandomGenerator>
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

    // 启动后先展示类别选择
    showCategorySelect();
}

void MainWindow::setupUi() {
    setMinimumSize(850, 650);
    resize(960, 700);
    setWindowTitle(QStringLiteral("钢铁意志：第三帝国的黄昏"));

    m_stackedWidget = new QStackedWidget(this);
    setCentralWidget(m_stackedWidget);

    m_menuWidget    = new MenuWidget(this);
    m_createWidget  = new CharacterCreateWidget(this);
    m_gameWidget    = new GameWidget(this);
    m_rpgGameWidget = new RpgGameWidget(this);

    m_stackedWidget->addWidget(m_menuWidget);       // 0
    m_stackedWidget->addWidget(m_createWidget);     // 1
    m_stackedWidget->addWidget(m_gameWidget);       // 2 - 剧情模式
    m_stackedWidget->addWidget(m_rpgGameWidget);    // 3 - 勇者模式

    showMainMenu();
}

void MainWindow::connectSignals() {
    // Menu
    connect(m_menuWidget, &MenuWidget::categorySelected, this, &MainWindow::onCategorySelected);
    connect(m_menuWidget, &MenuWidget::newGameClicked, this, &MainWindow::showCharacterCreate);
    connect(m_menuWidget, &MenuWidget::loadGameClicked, this, &MainWindow::openLoadDialog);
    connect(m_menuWidget, &MenuWidget::exitGameClicked, this, &MainWindow::close);
    connect(m_menuWidget, &MenuWidget::dlcSelected, this, &MainWindow::onDlcSelected);

    // Character create
    connect(m_createWidget, &CharacterCreateWidget::backToMenu, this, &MainWindow::showMainMenu);
    connect(m_createWidget, &CharacterCreateWidget::startGame, this, &MainWindow::onStartGame);

    // Narrative Game widget
    connect(m_gameWidget, &GameWidget::choiceMade, this, &MainWindow::onChoiceMade);
    connect(m_gameWidget, &GameWidget::saveClicked, this, &MainWindow::openSaveDialog);
    connect(m_gameWidget, &GameWidget::loadClicked, this, &MainWindow::openLoadDialog);
    connect(m_gameWidget, &GameWidget::exitClicked, this, &MainWindow::showMainMenu);

    // RPG Game widget
    connect(m_rpgGameWidget, &RpgGameWidget::choiceMade, this, &MainWindow::onChoiceMade);
    connect(m_rpgGameWidget, &RpgGameWidget::skillActivated, this, &MainWindow::onSkillActivated);
    connect(m_rpgGameWidget, &RpgGameWidget::saveClicked, this, &MainWindow::openSaveDialog);
    connect(m_rpgGameWidget, &RpgGameWidget::loadClicked, this, &MainWindow::openLoadDialog);
    connect(m_rpgGameWidget, &RpgGameWidget::exitClicked, this, &MainWindow::showMainMenu);

    // Set player reference for RPG widget
    m_rpgGameWidget->setPlayer(&m_player);

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
}

// --- Navigation ---

void MainWindow::showMainMenu() {
    m_stackedWidget->setCurrentWidget(m_menuWidget);
    m_musicPlayer->play(QStringLiteral("main_theme"));
}

void MainWindow::showCategorySelect() {
    m_menuWidget->showCategorySelect();
    m_stackedWidget->setCurrentWidget(m_menuWidget);
}

void MainWindow::onCategorySelected(const QString &category) {
    m_currentCategory = category;
    m_saveManager->setCategory(category);  // 分模式存档
    QList<DlcManifest> filtered = m_dlcManager->manifestsByCategory(category);
    m_menuWidget->showDlcList(filtered);
    m_stackedWidget->setCurrentWidget(m_menuWidget);
}

void MainWindow::showDlcSelect() {
    QList<DlcManifest> filtered = m_dlcManager->manifestsByCategory(m_currentCategory);
    m_menuWidget->showDlcList(filtered);
}

void MainWindow::showCharacterCreate() {
    m_stackedWidget->setCurrentWidget(m_createWidget);
}

void MainWindow::showGameScreen() {
    if (m_currentDlc.category == QStringLiteral("rpg")) {
        m_stackedWidget->setCurrentWidget(m_rpgGameWidget);
    } else {
        m_stackedWidget->setCurrentWidget(m_gameWidget);
    }
}

// --- DLC / Game Start ---

void MainWindow::onDlcSelected(const QString &dlcId) {
    const DlcManifest *m = m_dlcManager->getManifest(dlcId);
    if (!m || !m->valid) return;

    m_currentDlc = *m;
    m_currentDlcBasePath = m_dlcManager->dlcBasePath(dlcId);
    registerMusicFromDlc();

    // 将DLC的职业列表传给角色创建界面（传入类别）
    m_createWidget->setClasses(m_currentDlc.classes, m_currentDlc.category);

    // 选定DLC后回到主菜单
    m_menuWidget->showMainMenu(m_currentDlc.title, m_currentDlc.subtitle, m_currentDlc.category);
    m_stackedWidget->setCurrentWidget(m_menuWidget);
    m_musicPlayer->play(QStringLiteral("main_theme"));
}

void MainWindow::onStartGame(const QString &name, const QString &classId) {
    m_player = PlayerSystem(name, classId, m_currentDlc.dlcId);
    m_player.currentChapter = m_currentDlc.startChapter;
    m_player.unlockChapter(m_currentDlc.startChapter);

    // Find class info
    m_currentClassName = classId;
    for (const auto &c : m_currentDlc.classes) {
        if (c.id == classId) {
            m_currentClassName = c.name;
            // RPG模式：从职业读取初始属性
            if (m_currentDlc.category == QStringLiteral("rpg")) {
                m_player.maxHp   = c.baseHp;
                m_player.hp      = c.baseHp;
                m_player.maxMp   = c.baseInt * 10;
                m_player.mp      = m_player.maxMp;
                m_player.baseStr = c.baseStr;
                m_player.baseAgi = c.baseAgi;
                m_player.baseInt = c.baseInt;
                m_player.level   = 1;
                m_player.exp     = 0;
                // 职业初始武器
                QString startWeapon;
                if (c.id == "hero")      startWeapon = QStringLiteral("武器:铁剑");
                else if (c.id == "warrior") startWeapon = QStringLiteral("武器:铁拳套");
                else if (c.id == "archer")  startWeapon = QStringLiteral("武器:猎弓");
                else if (c.id == "mage")    startWeapon = QStringLiteral("武器:学徒法杖");
                else if (c.id == "priest")  startWeapon = QStringLiteral("武器:圣光之书");
                m_player.addItem(startWeapon);
                m_player.addItem(QStringLiteral("防具:布衣"));
                // 初始道具：1治疗+1魔力
                m_player.addItem(QStringLiteral("治疗药水"));
                m_player.addItem(QStringLiteral("魔力药水"));
                m_player.setQuickItem(0, QStringLiteral("治疗药水"));
                m_player.setQuickItem(1, QStringLiteral("魔力药水"));
                // 设置技能
                m_rpgGameWidget->setSkills(c.skills);
            }
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

void MainWindow::onSkillActivated(int choiceIdx, int combatBonus) {
    m_nodeEngine->setCombatModifier(combatBonus);
    m_nodeEngine->makeChoice(choiceIdx);
}

void MainWindow::onChoiceMade(int index) {
    if (index == -2) {
        const QList<DlcChapterMeta> &chapters = m_currentDlc.chapters;
        QString currentCh = m_player.currentChapter;
        for (int i = 0; i < chapters.size(); ++i) {
            if (chapters[i].id == currentCh && i + 1 < chapters.size()) {
                QString nextChId = chapters[i + 1].id;
                // RPG 模式：通关奖励
                if (m_currentDlc.category == QStringLiteral("rpg")) {
                    m_player.level += 4;
                    m_player.exp += 100;
                    m_player.maxHp += 15;
                    m_player.hp = m_player.maxHp;
                    m_player.maxMp = m_player.baseInt * 10 + m_player.level * 2;
                    m_player.mp = m_player.maxMp;

                    // Class-biased stat growth: 4 levels × 1 stat point each
                    for (int l = 0; l < 4; l++) {
                        int r = QRandomGenerator::global()->bounded(100);
                        if (m_player.classId == "hero") {
                            // Hero: balanced (Str 40%, Agi 30%, Int 30%)
                            if (r < 40) m_player.baseStr++;
                            else if (r < 70) m_player.baseAgi++;
                            else m_player.baseInt++;
                        } else if (m_player.classId == "warrior") {
                            // Warrior: Str-heavy (Str 60%, Agi 25%, Int 15%)
                            if (r < 60) m_player.baseStr++;
                            else if (r < 85) m_player.baseAgi++;
                            else m_player.baseInt++;
                        } else if (m_player.classId == "archer") {
                            // Archer: Agi+Str (Agi 45%, Str 35%, Int 20%)
                            if (r < 45) m_player.baseAgi++;
                            else if (r < 80) m_player.baseStr++;
                            else m_player.baseInt++;
                        } else if (m_player.classId == "mage") {
                            // Mage: Int-heavy (Int 60%, Agi 25%, Str 15%)
                            if (r < 60) m_player.baseInt++;
                            else if (r < 85) m_player.baseAgi++;
                            else m_player.baseStr++;
                        } else if (m_player.classId == "priest") {
                            // Priest: Int+balanced (Int 50%, Str 25%, Agi 25%)
                            if (r < 50) m_player.baseInt++;
                            else if (r < 75) m_player.baseStr++;
                            else m_player.baseAgi++;
                        }
                    }

                    // === Probability-based loot: weapon + armor + accessory ===
                    int chIdx = 1;
                    for (int ci = 0; ci < m_currentDlc.chapters.size(); ci++) {
                        if (m_currentDlc.chapters[ci].id == m_player.currentChapter) {
                            chIdx = ci + 1; break;
                        }
                    }

                    // ---- Weapon pool (sum=1, T1≥50%, T4≤50%) ----
                    QStringList wpnPool;
                    QList<int> wpnRates;  // T1,T2,T3,T4 weights sum to 100
                    if (m_player.classId == "hero") {
                        wpnPool = {"武器:铁剑","武器:钢剑","武器:圣剑","武器:传说之剑"};
                    } else if (m_player.classId == "warrior") {
                        wpnPool = {"武器:铁拳套","武器:战拳套","武器:狂战拳套","武器:泰坦拳套"};
                    } else if (m_player.classId == "archer") {
                        wpnPool = {"武器:猎弓","武器:长弓","武器:精灵弓","武器:风暴弓"};
                    } else if (m_player.classId == "mage") {
                        wpnPool = {"武器:学徒法杖","武器:奥术法杖","武器:长老法杖","武器:大法师杖"};
                    } else {
                        wpnPool = {"武器:圣光之书","武器:圣光法典","武器:圣光圣典","武器:神圣启示录"};
                    }
                    // Weapon rates: T1≥50%, all sum to 100
                    if (chIdx == 1)      wpnRates = {70, 20,  8,  2};
                    else if (chIdx == 2) wpnRates = {60, 20, 15,  5};
                    else if (chIdx == 3) wpnRates = {55, 15, 17, 13};
                    else                 wpnRates = {50, 10, 15, 25};

                    // ---- Armor pool (sum=1) ----
                    QStringList armPool = {"防具:布衣","防具:皮甲","防具:链甲","防具:板甲"};
                    QList<int> armRates;
                    if (chIdx == 1)      armRates = {75, 20,  5,  0};
                    else if (chIdx == 2) armRates = {60, 25, 12,  3};
                    else if (chIdx == 3) armRates = {55, 20, 18,  7};
                    else                 armRates = {50, 15, 20, 15};

                    // ---- Accessory pool (sum=1) ----
                    QStringList accPool = {"饰品:守护戒指","饰品:智慧项链","饰品:敏捷护符","饰品:龙鳞挂饰"};
                    QList<int> accRates;
                    if (chIdx == 1)      accRates = {70, 15, 15,  0};
                    else if (chIdx == 2) accRates = {55, 20, 20,  5};
                    else if (chIdx == 3) accRates = {45, 20, 20, 15};
                    else                 accRates = {35, 20, 20, 25};

                    // Roll helper
                    auto rollDrop = [](const QStringList &pool, const QList<int> &rates) -> QString {
                        int r = QRandomGenerator::global()->bounded(100);
                        int cum = 0;
                        for (int i = 0; i < rates.size() && i < pool.size(); i++) {
                            cum += rates[i];
                            if (r < cum) return pool[i];
                        }
                        return pool.last();
                    };

                    QString wpn = rollDrop(wpnPool, wpnRates);
                    QString arm = rollDrop(armPool, armRates);
                    QString acc = rollDrop(accPool, accRates);
                    m_player.addItem(wpn);
                    m_player.addItem(arm);
                    m_player.addItem(acc);

                    // Build drop info string
                    auto ratesStr = [](const QStringList &pool, const QList<int> &rates) -> QString {
                        QString s;
                        for (int i = 0; i < pool.size() && i < rates.size(); i++)
                            s += pool[i].mid(3) + ": " + QString::number(rates[i]) + "%  ";
                        return s.trimmed();
                    };
                    QMessageBox::information(this, QStringLiteral("战利品掉落 (第%1章)").arg(chIdx),
                        QStringLiteral("⚔ 武器: %1\n🛡 防具: %2\n💍 饰品: %3\n\n"
                        "【武器概率】%4\n【防具概率】%5\n【饰品概率】%6")
                        .arg(wpn.mid(3)).arg(arm.mid(3)).arg(acc.mid(3))
                        .arg(ratesStr(wpnPool, wpnRates))
                        .arg(ratesStr(armPool, armRates))
                        .arg(ratesStr(accPool, accRates)));

                    // 1 mana potion guaranteed, 30% chance of health potion
                    m_player.addItem(QStringLiteral("魔力药水"));
                    if (QRandomGenerator::global()->bounded(100) < 30) {
                        m_player.addItem(QStringLiteral("治疗药水"));
                    }
                    // Set first empty quick slot to mana potion
                    for (int s = 0; s < 4; s++) {
                        if (s >= m_player.quickItems.size() || m_player.quickItems[s].isEmpty()) {
                            m_player.setQuickItem(s, QStringLiteral("魔力药水"));
                            break;
                        }
                    }
                    for (const auto &c : m_currentDlc.classes) {
                        if (c.id == m_player.classId) {
                            m_rpgGameWidget->setSkills(c.skills);
                            break;
                        }
                    }
                }
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

    if (m_currentDlc.category == QStringLiteral("rpg")) {
        for (const auto &c : m_currentDlc.classes) {
            if (c.id == m_player.classId) {
                m_rpgGameWidget->setSkills(c.skills);
                break;
            }
        }
        m_rpgGameWidget->syncEquipFlags();  // Sync weapon→flags for JSON gating
        m_rpgGameWidget->showStoryNode(node, m_player.classId, chName, isLastChapter);
        m_rpgGameWidget->updatePlayerStats(m_player.hp, m_player.maxHp, m_player.mp, m_player.maxMp);
        m_rpgGameWidget->updatePlayerInfo(m_player.name, m_currentClassName, m_player.level, m_player.exp);
        m_rpgGameWidget->updateEquipment(m_player.equipment);
        m_rpgGameWidget->updateQuickItems(m_player.quickItems);
        m_rpgGameWidget->updateInventory(m_player.inventory);
    } else {
        m_gameWidget->showStoryNode(node, m_player.classId, chName, isLastChapter);
        m_gameWidget->updatePlayerStats(m_player.hp, m_player.morale);
        m_gameWidget->updatePlayerInfo(m_player.name, m_currentClassName);
    }

    if (!node->musicKey.isEmpty())
        m_musicPlayer->play(node->musicKey);

    m_saveManager->autoSave(m_player, m_currentDlc.title,
                            m_currentClassName, chName);
}

void MainWindow::onStatsChanged(int hp, int morale) {
    if (m_currentDlc.category == QStringLiteral("rpg")) {
        m_rpgGameWidget->updatePlayerStats(m_player.hp, m_player.maxHp, m_player.mp, m_player.maxMp);
    } else {
        m_gameWidget->updatePlayerStats(hp, morale);
    }
}

void MainWindow::onCombatResult(bool success, int hpChange, int moraleChange) {
    QString title = success ? QStringLiteral("判定成功") : QStringLiteral("判定失败");
    QString msg;
    if (m_currentDlc.category == QStringLiteral("rpg")) {
        msg = success
            ? QStringLiteral("【战斗胜利！】\n你成功击败了敌人！")
            : QStringLiteral("【战斗失败！】\n你受到了 %1 点伤害，士气下降了 %2 点。")
              .arg(qAbs(hpChange)).arg(qAbs(moraleChange));
    } else {
        msg = success
            ? QStringLiteral("【判定结果：成功】\n您通过敏捷的动作或精湛的特技成功化险为夷！")
            : QStringLiteral("【判定结果：失败】\n局势恶化！您在交火或事故中遭受了重创。\n生命值损失：%1 | 士气值损失：%2")
              .arg(qAbs(hpChange)).arg(qAbs(moraleChange));
    }
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
