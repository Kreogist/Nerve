#ifndef KNMUSICPLAYLISTSONGS_H
#define KNMUSICPLAYLISTSONGS_H

#include "knmusiclistviewbase.h"

class KNMusicPlaylistSongs : public KNMusicListViewBase
{
    Q_OBJECT
public:
    explicit KNMusicPlaylistSongs(QWidget *parent = 0);

signals:

public slots:

private slots:
    void onItemActived(const QModelIndex &index);
};

#endif // KNMUSICPLAYLISTSONGS_H
