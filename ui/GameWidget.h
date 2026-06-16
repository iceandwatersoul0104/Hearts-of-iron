#pragma once
#include <QWidget>
#include <QList>
#include "../engine/DlcTypes.h"

class QTextBrowser;
class QPushButton;
class QProgressBar;
class QLabel;
class QTimer;
class QVBoxLayout;

class GameWidget : public QWidget {
    Q_OBJECT
public:
    explicit GameWidget(QWidget *parent = nullptr);
    ~GameWidget() = default;

    void updatePlayerStats(int hp, int morale);
    void updatePlayerInfo(const QString &name, const QString &className);
    void showStoryNode(const StoryNode *node, const QString &playerClassId,
                       const QString &chapterName, bool isLastChapter);

signals:
    void choiceMade(int index);
    void saveClicked();
    void loadClicked();
    void exitClicked();

private slots:
    void onTypeTimerTick();
    void onChoiceButtonClicked();
    void skipTypewriter();

protected:
    bool eventFilter(QObject *obj, QEvent *event) override;

private:
    void setupUi();
    void displayFullText();

    QTextBrowser *m_textBrowser = nullptr;
    QProgressBar *m_hpBar = nullptr;
    QProgressBar *m_moraleBar = nullptr;

    QLabel *m_nameLabel = nullptr;
    QLabel *m_classLabel = nullptr;
    QLabel *m_locationLabel = nullptr;
    QLabel *m_scenarioLabel = nullptr;

    QWidget *m_optionsContainer = nullptr;
    QVBoxLayout *m_optionsLayout = nullptr;
    QList<QPushButton*> m_optionButtons;

    QPushButton *m_saveBtn = nullptr;
    QPushButton *m_loadBtn = nullptr;
    QPushButton *m_exitBtn = nullptr;

    QString m_fullText;
    int m_currentIndex = 0;
    QTimer *m_typeTimer = nullptr;
    bool m_isTyping = false;
};
