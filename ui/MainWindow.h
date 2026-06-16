#pragma once
#include <QMainWindow>
#include "../engine/DlcTypes.h"
#include "../engine/PlayerSystem.h"

class QStackedWidget;
class DlcManager;
class NodeEngine;
class DiceSystem;
class SaveManager;
class MusicPlayer;
class MenuWidget;
class CharacterCreateWidget;
class GameWidget;
struct StoryNode;
struct DlcManifest;

class MainWindow : public QMainWindow {
    Q_OBJECT
public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow() = default;

private slots:
    void showMainMenu();
    void showDlcSelect();
    void showCharacterCreate();
    void showGameScreen();

    void onDlcSelected(const QString &dlcId);
    void onStartGame(const QString &name, const QString &classId);
    void onLoadGame(const PlayerSystem &loadedPlayer);

    void onChoiceMade(int index);

    void openSaveDialog();
    void openLoadDialog();

    void onNodeChanged(const StoryNode *node);
    void onStatsChanged(int hp, int morale);
    void onCombatResult(bool success, int hpChange, int moraleChange);
    void onChapterVictory(const QString &chapterId);
    void onChapterDefeat(const QString &chapterId);

private:
    void setupUi();
    void connectSignals();
    void registerMusicFromDlc();
    void startChapterMusic();

    QStackedWidget *m_stackedWidget = nullptr;

    MenuWidget *m_menuWidget = nullptr;
    CharacterCreateWidget *m_createWidget = nullptr;
    GameWidget *m_gameWidget = nullptr;

    DlcManager  *m_dlcManager  = nullptr;
    DiceSystem  *m_diceSystem  = nullptr;
    NodeEngine  *m_nodeEngine  = nullptr;
    SaveManager *m_saveManager = nullptr;
    MusicPlayer *m_musicPlayer = nullptr;

    PlayerSystem m_player;
    DlcManifest  m_currentDlc;
    QString      m_currentDlcBasePath;
    QString      m_currentClassName;
    QString      m_currentChapterName;
};
