#include "knmusicalbumsonglistview.h"

KNMusicAlbumSongListView::KNMusicAlbumSongListView(QWidget *parent) :
    KNMusicListViewBase(parent)
{
    QPalette pal=palette();
    pal.setColor(QPalette::Base, QColor(0,0,0,0));
    setPalette(pal);
}
