#pragma once
#include <QMainWindow>
#include "../core/GameState.h"
#include "../core/Player.h"

class QStackedWidget;
class GameEngine;
class SaveManager;
class MusicPlayer;
class MenuWidget;
class CharacterCreateWidget;
class GameWidget;
struct StoryNode;

class MainWindow : public QMainWindow {
    Q_OBJECT
public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow() = default;

private slots:
    void showMainMenu();
    void showCharacterCreate();
    void showGameScreen();
    
    void onStartGame(const QString &name, PlayerClass cls);
    void onLoadGame(const Player &loadedPlayer);
    
    void onChoiceMade(int index);
    
    void openSaveDialog();
    void openLoadDialog();
    
    void onNodeChanged(const StoryNode *node);
    void onStatsChanged(int hp, int morale);
    void onCombatResult(bool success, int hpChange, int moraleChange);
    void onScenarioVictory(ScenarioId id);
    void onScenarioDefeat(ScenarioId id);

private:
    void setupUi();
    void connectSignals();
    void playScenarioMusic(ScenarioId id);
    void initializeMusicTracks();

    QStackedWidget *m_stackedWidget = nullptr;
    
    MenuWidget *m_menuWidget = nullptr;
    CharacterCreateWidget *m_createWidget = nullptr;
    GameWidget *m_gameWidget = nullptr;

    GameEngine *m_engine = nullptr;
    SaveManager *m_saveManager = nullptr;
    MusicPlayer *m_musicPlayer = nullptr;
};
