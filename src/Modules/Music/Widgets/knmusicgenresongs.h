#ifndef KNMUSICGENRESONGS_H
#define KNMUSICGENRESONGS_H

#include "knmusiclistview.h"

class KNMusicGenreSongs : public KNMusicListView
{
    Q_OBJECT
public:
    explicit KNMusicGenreSongs(QWidget *parent = 0);
    void resetHeader();

signals:

public slots:

};

#endif // KNMUSICGENRESONGS_H
