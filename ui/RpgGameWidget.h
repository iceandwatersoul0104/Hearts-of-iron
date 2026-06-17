#pragma once
#include <QWidget>
#include <QTextBrowser>
#include <QProgressBar>
#include <QPushButton>
#include <QLabel>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QTimer>
#include <QMap>
#include "../engine/DlcTypes.h"
#include "../engine/PlayerSystem.h"

class RpgGameWidget : public QWidget {
    Q_OBJECT
public:
    explicit RpgGameWidget(QWidget *parent = nullptr);

    void setPlayer(PlayerSystem *player) { m_player = player; }

    void showStoryNode(const StoryNode *node, const QString &playerClassId,
                       const QString &chapterName, bool isLastChapter);
    void updatePlayerStats(int hp, int maxHp, int mp, int maxMp);
    void updatePlayerInfo(const QString &name, const QString &className,
                          int level, int exp);
    void updateEquipment(const QMap<QString, QString> &equipment);
    void updateQuickItems(const QStringList &quickItems);
    void updateInventory(const QStringList &inventory);
    void setSkills(const QList<Skill> &skills);
    void syncEquipFlags();

signals:
    void choiceMade(int index);
    void saveClicked();
    void loadClicked();
    void exitClicked();
    void skillActivated(int choiceIdx, int combatBonus);
    void itemUsed(const QString &itemName);

protected:
    bool eventFilter(QObject *obj, QEvent *event) override;

private slots:
    void onChoiceButtonClicked();
    void onTypeTimerTick();
    void onQuickItemClicked();
    void onSkillButtonClicked();
    void onEquipSlotClicked();

private:
    void setupUi();
    void skipTypewriter();
    void displayFullText();
    void clearOptionButtons();
    void rebuildInventoryPanel();
    void showStoryNode_impl(const StoryNode *node);
    void showBackpackDialog();
    void getEquipStats(const QString &itemName, int &str, int &agi, int &intel, int &hp, int &mp);

    PlayerSystem *m_player = nullptr;

    // Typewriter
    QTimer *m_typeTimer = nullptr;
    QString m_fullText;
    int     m_charIndex = 0;
    bool    m_isTyping = false;

    // Header
    QLabel *m_nameLabel = nullptr;
    QLabel *m_classLabel = nullptr;
    QLabel *m_levelLabel = nullptr;
    QLabel *m_expLabel = nullptr;

    // Stats
    QProgressBar *m_hpBar = nullptr;
    QProgressBar *m_mpBar = nullptr;
    QLabel *m_hpLabel = nullptr;
    QLabel *m_mpLabel = nullptr;

    // Equipment
    QPushButton *m_weaponBtn = nullptr;
    QPushButton *m_armorBtn = nullptr;
    QPushButton *m_accessoryBtn = nullptr;

    // Quick Items
    QPushButton *m_quickBtns[4] = {};

    // Inventory
    QWidget *m_invPanel = nullptr;
    QVBoxLayout *m_invLayout = nullptr;
    QPushButton *m_invToggleBtn = nullptr;
    bool m_invExpanded = false;

    // Stats display (sidebar)
    QLabel *m_strLabel = nullptr;
    QLabel *m_agiLabel = nullptr;
    QLabel *m_intLabel = nullptr;

    // Skills (for combat)
    QList<Skill> m_skills;

    // Story
    QTextBrowser *m_textBrowser = nullptr;

    // Options
    QWidget *m_optionsContainer = nullptr;
    QVBoxLayout *m_optionsLayout = nullptr;

    // Controls
    QPushButton *m_saveBtn = nullptr;
    QPushButton *m_loadBtn = nullptr;
    QPushButton *m_exitBtn = nullptr;

    // State
    QString m_playerClassId;
    bool    m_isLastChapter = false;
    int     m_pendingChoiceIdx = -1;  // choice index after skill is activated
};
