#include <QScrollBar>

#include "knmusicalbumview.h"

KNMusicAlbumView::KNMusicAlbumView(QWidget *parent) :
    QListView(parent)
{
    setViewMode(IconMode);
    setIconSize(QSize(128, 128));
    setGridSize(QSize(148, 148));
    setResizeMode(Adjust);
    setMovement(Static);

    horizontalScrollBar()->setPageStep(4);
    verticalScrollBar()->setPageStep(4);
}
