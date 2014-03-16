#include <QLabel>
#include <QDebug>

#include "Modules/Music/Widgets/knmusicviewer.h"

#include "mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent)
{
    KNMusicViewer *test=new KNMusicViewer(this);
    setCentralWidget(test);
}
