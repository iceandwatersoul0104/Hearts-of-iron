#pragma once
#include <QWidget>
#include <QList>
#include "../engine/DlcTypes.h"

class QPushButton;
class QStackedWidget;
class QVBoxLayout;
class QLabel;

class MenuWidget : public QWidget {
    Q_OBJECT
public:
    explicit MenuWidget(QWidget *parent = nullptr);
    ~MenuWidget() = default;

    void showCategorySelect();
    void showDlcList(const QList<DlcManifest> &manifests);
    void showMainMenu(const QString &title, const QString &subtitle, const QString &category = QString());

signals:
    void categorySelected(const QString &category);
    void newGameClicked();
    void loadGameClicked();
    void exitGameClicked();
    void dlcSelected(const QString &dlcId);

private:
    void setupUi();
    void setupCategoryPanel();
    void setupDlcListPanel();

    QStackedWidget *m_stack = nullptr;

    // Category selection panel (index 0)
    QWidget *m_categoryPanel = nullptr;

    // Main menu panel (index 1)
    QWidget *m_mainMenuPanel = nullptr;
    QLabel *m_mainTitle = nullptr;
    QLabel *m_mainSubtitle = nullptr;
    QPushButton *m_newGameBtn = nullptr;
    QPushButton *m_loadGameBtn = nullptr;
    QPushButton *m_exitGameBtn = nullptr;
    QPushButton *m_dlcBackBtn = nullptr;

    // DLC list panel (index 2)
    QWidget *m_dlcListPanel = nullptr;
    QVBoxLayout *m_dlcListLayout = nullptr;
    QPushButton *m_categoryBackBtn = nullptr;
};
