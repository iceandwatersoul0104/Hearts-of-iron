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

    void showDlcList(const QList<DlcManifest> &manifests);
    void showMainMenu(const QString &title, const QString &subtitle);

signals:
    void newGameClicked();
    void loadGameClicked();
    void exitGameClicked();
    void dlcSelected(const QString &dlcId);

private:
    void setupUi();
    void setupDlcListPanel();

    QStackedWidget *m_stack = nullptr;

    // Main menu panel
    QWidget *m_mainMenuPanel = nullptr;
    QLabel *m_mainTitle = nullptr;
    QLabel *m_mainSubtitle = nullptr;
    QPushButton *m_newGameBtn = nullptr;
    QPushButton *m_loadGameBtn = nullptr;
    QPushButton *m_exitGameBtn = nullptr;

    // DLC list panel
    QWidget *m_dlcListPanel = nullptr;
    QVBoxLayout *m_dlcListLayout = nullptr;
};
