#include <QDebug>

#include <QLabel>
#include "Modules/Music/Libraries/knmusictagid3v2.h"

#include "knstdlibviewer.h"

#include "mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent)
{
    setAutoFillBackground(true);
    KNMusicTagID3v2 *test=new KNMusicTagID3v2(this);
    setCentralWidget(imageTestLabel);
    test->readTag("D:/01. Won(＊3＊)Chu KissMe!.mp3");
}
