#ifndef KNMUSICGENRESONGS_H
#define KNMUSICGENRESONGS_H

#include "knmusiclistviewbase.h"

class KNMusicGenreSongs : public KNMusicListViewBase
{
    Q_OBJECT
public:
    explicit KNMusicGenreSongs(QWidget *parent = 0);
    void resetHeader();

signals:

public slots:

};

#endif // KNMUSICGENRESONGS_H
