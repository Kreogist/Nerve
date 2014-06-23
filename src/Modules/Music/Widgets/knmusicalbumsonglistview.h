#ifndef KNMUSICALBUMSONGLISTVIEW_H
#define KNMUSICALBUMSONGLISTVIEW_H

#include "knmusiclibrarylistview.h"

class KNMusicAlbumSongListView : public KNMusicLibraryListview
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
