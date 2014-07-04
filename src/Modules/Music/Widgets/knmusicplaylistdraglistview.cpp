#include <QMimeData>
#include <QDragEnterEvent>

#include <QDebug>

#include "knmusicplaylistdraglistview.h"

KNMusicPlaylistDragListView::KNMusicPlaylistDragListView(QWidget *parent) :
    KNMusicPlaylistListView(parent)
{
    //Resize the listview.
    resize(200, 100);
}
