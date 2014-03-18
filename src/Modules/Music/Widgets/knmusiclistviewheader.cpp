#include "knmusiclistviewheader.h"

KNMusicListViewHeader::KNMusicListViewHeader(QWidget *parent) :
    QHeaderView(Qt::Horizontal, parent)
{
    ;
}

void KNMusicListViewHeader::moveToFirst(int logicalIndex)
{
    moveSection(visualIndex(logicalIndex), 0);
}
