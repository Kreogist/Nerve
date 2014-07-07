#include <QMap>

#include <QDebug>

#include "knmusicplaylistsongs.h"

KNMusicPlaylistSongs::KNMusicPlaylistSongs(QWidget *parent) :
    KNMusicListViewBase(parent)
{
    setDragDropMode(QAbstractItemView::InternalMove);
    setSelectionBehavior(QAbstractItemView::SelectRows);
    setSortingEnabled(false);
    connect(this, &KNMusicPlaylistSongs::activated,
            this, &KNMusicPlaylistSongs::onItemActived);
}

void KNMusicPlaylistSongs::onItemActived(const QModelIndex &index)
{
    if(index.isValid())
    {
        emit requireOpenUrl(index);
    }
}
