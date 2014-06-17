#include "knmusicplaylistsongs.h"

KNMusicPlaylistSongs::KNMusicPlaylistSongs(QWidget *parent) :
    QTreeView(parent)
{
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
