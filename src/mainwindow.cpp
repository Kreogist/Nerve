#include <QDebug>

#include "Modules/Base/kncutestyle.h"
#include "Modules/Music/Libraries/knmusictagid3v2.h"
#include "Modules/Music/Widgets/knmusicviewer.h"

#include "mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent)
{
    //Set style.
    KNCuteStyle *style=new KNCuteStyle;
    setStyle(style);

    KNMusicViewer *musicViewer=new KNMusicViewer(this);
    setCentralWidget(musicViewer);
}
