#include <QMap>

#include <QDebug>

#include "knmusicplaylistheader.h"

#include "knmusicplaylistsongs.h"

KNMusicPlaylistSongs::KNMusicPlaylistSongs(QWidget *parent) :
    KNMusicListViewBase(parent)
{
    //Set properties.
    setDragDropMode(QAbstractItemView::InternalMove);
    setSelectionBehavior(QAbstractItemView::SelectRows);
    setSortingEnabled(false);

    //Set header.
    m_headerWidget=new KNMusicPlaylistHeader(this);
    m_headerWidget->installEventFilter(this);
    setHeader(m_headerWidget);

    //Set activate signals.
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
