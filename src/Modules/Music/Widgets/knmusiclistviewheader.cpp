#include "knmusiclistviewheader.h"

KNMusicListViewHeader::KNMusicListViewHeader(QWidget *parent) :
    QHeaderView(Qt::Horizontal, parent)
{
   setSectionsMovable(true);
   setDefaultAlignment(Qt::AlignLeft);
}

void KNMusicListViewHeader::moveToFirst(int logicalIndex)
{
    moveSection(visualIndex(logicalIndex), 0);
}
