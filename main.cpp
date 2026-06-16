#include <QApplication>
#include <QFile>
#include "ui/MainWindow.h"

int main(int argc, char *argv[]) {
    QApplication a(argc, argv);

    // 加载 QSS 样式表
    QFile file(QStringLiteral(":/style.qss"));
    if (file.open(QFile::ReadOnly | QFile::Text)) {
        a.setStyleSheet(QString::fromUtf8(file.readAll()));
    }

    MainWindow w;
    w.show();

    return a.exec();
}
