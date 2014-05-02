#ifndef KNMUSICARTISTSONGS_H
#define KNMUSICARTISTSONGS_H

#include "knmusiclistview.h"

class KNMusicArtistSongs : public KNMusicListView
{
    Q_OBJECT
public:
    explicit KNMusicArtistSongs(QWidget *parent = 0);
    void resetHeader();

signals:

public slots:

};

#endif // KNMUSICARTISTSONGS_H
