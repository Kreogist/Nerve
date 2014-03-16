#include <QDebug>

#include "Modules/Music/Libraries/knmusictagid3v2.h"

#include "knstdlibviewer.h"

#include "mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent)
{
    KNMusicTagID3v2 *test=new KNMusicTagID3v2(this);
    test->readTag("D:/01. Won(＊3＊)Chu KissMe!_ID3v2.mp3");
}
