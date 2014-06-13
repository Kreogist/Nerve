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
    ;
}

void KNMusicPlaylistViewer::resetHeader()
{
    ;
}
