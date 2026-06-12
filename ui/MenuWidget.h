#pragma once
#include <QWidget>

class QPushButton;

class MenuWidget : public QWidget {
    Q_OBJECT
public:
    explicit MenuWidget(QWidget *parent = nullptr);
    ~MenuWidget() = default;

signals:
    void newGameClicked();
    void loadGameClicked();
    void exitGameClicked();

private:
    void setupUi();

    QPushButton *m_newGameBtn = nullptr;
    QPushButton *m_loadGameBtn = nullptr;
    QPushButton *m_exitGameBtn = nullptr;
};
