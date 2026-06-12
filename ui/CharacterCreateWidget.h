#pragma once
#include <QWidget>
#include "../core/GameState.h"

class QLineEdit;
class QLabel;
class QButtonGroup;
class QPushButton;

class CharacterCreateWidget : public QWidget {
    Q_OBJECT
public:
    explicit CharacterCreateWidget(QWidget *parent = nullptr);
    ~CharacterCreateWidget() = default;

signals:
    void backToMenu();
    void startGame(const QString &name, PlayerClass cls);

private slots:
    void onClassSelected(int id);
    void onStartClicked();

private:
    void setupUi();
    void updatePreview(PlayerClass cls);

    QLineEdit *m_nameEdit = nullptr;
    QButtonGroup *m_classGroup = nullptr;
    QList<QWidget*> m_classCards;
    
    QLabel *m_hpLabel = nullptr;
    QLabel *m_moraleLabel = nullptr;
    QLabel *m_descLabel = nullptr;
    QLabel *m_scenarioLabel = nullptr;

    QPushButton *m_startBtn = nullptr;
    QPushButton *m_backBtn = nullptr;

    PlayerClass m_selectedClass = PlayerClass::Infantry;
};
