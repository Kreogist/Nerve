#include "knmusicartistitem.h"

KNMusicArtistItem::KNMusicArtistItem(QString name) :
    QStandardItem(name)
{
}

void KNMusicArtistItem::addSongs(const QModelIndex &index)
{
    m_songs.append(index);
}

QList<QModelIndex> KNMusicArtistItem::songs() const
{
    return m_songs;
}

void KNMusicArtistItem::setSongs(const QList<QModelIndex> &songs)
{
    m_songs = songs;
}


