#pragma once
#include <QWidget>
#include <QList>
#include "../engine/DlcTypes.h"

class QLineEdit;
class QLabel;
class QGridLayout;
class QPushButton;

class CharacterCreateWidget : public QWidget {
    Q_OBJECT
public:
    explicit CharacterCreateWidget(QWidget *parent = nullptr);
    ~CharacterCreateWidget() = default;

    // Set classes from DLC manifest (called before showing)
    void setClasses(const QList<DlcClass> &classes, const QString &category = QString());

signals:
    void backToMenu();
    void startGame(const QString &name, const QString &classId);

protected:
    bool eventFilter(QObject *obj, QEvent *event) override;

private slots:
    void onClassSelected(int id);
    void onStartClicked();

private:
    void setupUi();
    void updatePreview(const DlcClass &cls);
    void rebuildClassCards();

    QLineEdit *m_nameEdit = nullptr;
    QGridLayout *m_grid = nullptr;
    QList<QWidget*> m_classCards;
    QList<DlcClass> m_classes;
    QString m_category;

    QLabel *m_titleLabel = nullptr;
    QLabel *m_nameTipLabel = nullptr;
    QLabel *m_classTipLabel = nullptr;
    QLabel *m_previewTitleLabel = nullptr;
    QLabel *m_hpLabel = nullptr;
    QLabel *m_moraleLabel = nullptr;   // RPG模式下显示MP
    QLabel *m_descLabel = nullptr;
    QLabel *m_scenarioLabel = nullptr;

    QPushButton *m_startBtn = nullptr;
    QPushButton *m_backBtn = nullptr;

    int m_selectedIndex = 0;
};
