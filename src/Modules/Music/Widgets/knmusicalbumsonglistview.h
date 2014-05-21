#ifndef KNMUSICALBUMSONGLISTVIEW_H
#define KNMUSICALBUMSONGLISTVIEW_H

#include "knmusiclistviewbase.h"

class KNMusicAlbumSongListView : public KNMusicListViewBase
{
    Q_OBJECT
public:
    explicit KNMusicAlbumSongListView(QWidget *parent = 0);
    void resetHeader();
    void resizeHeader();

signals:

public slots:

private:
    int m_widthTweak=5;
};

#endif // KNMUSICALBUMSONGLISTVIEW_H
