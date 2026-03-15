#include "mainwindow.h"

#include <QApplication>
#include <QScreen>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    w.move(QGuiApplication::primaryScreen()->availableGeometry().center() - w.rect().center());
    return a.exec();
}
