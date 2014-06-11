#include <QHeaderView>

#include <QDebug>

#include "../knmusicglobal.h"

#include "knmusicplaylistviewer.h"

KNMusicPlaylistViewer::KNMusicPlaylistViewer(QWidget *parent) :
    QTreeView(parent)
{
    //Set basic properties for playlist viewer.
    setAllColumnsShowFocus(true);
    setIndentation(0);
    setUniformRowHeights(true);
    setVerticalScrollMode(QAbstractItemView::ScrollPerPixel);
    //Set palette.
    QPalette pal=palette();
    pal.setColor(QPalette::Base, QColor(0,0,0,0));
    pal.setColor(QPalette::Window, QColor(0,0,0,0));
    pal.setColor(QPalette::Text, QColor(0xff,0xff,0xff));
    pal.setColor(QPalette::WindowText, QColor(0xff,0xff,0xff));
    pal.setColor(QPalette::ButtonText, QColor(0xff,0xff,0xff));
    setPalette(pal);
    header()->setVisible(false);
}

void KNMusicPlaylistViewer::resetHeader()
{
    setAlternatingRowColors(false);
    for(int i=KNMusicGlobal::Name+1;
            i<KNMusicGlobal::MusicDataCount;
            i++)
    {
        setColumnHidden(i, true);
    };
    setColumnHidden(KNMusicGlobal::Name, false);
    setColumnHidden(KNMusicGlobal::Time, false);
}
