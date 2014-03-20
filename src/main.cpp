#include <QApplication>

#include <QStyleFactory>

#include "mainwindow.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    app.setStyle(QStyleFactory::create("fusion"));
    MainWindow mainWindow;
    mainWindow.show();
    return app.exec();
}
