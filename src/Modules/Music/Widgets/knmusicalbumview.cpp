#include "knmusicalbumview.h"

KNMusicAlbumView::KNMusicAlbumView(QWidget *parent) :
    QListView(parent)
{
    setViewMode(IconMode);
}
