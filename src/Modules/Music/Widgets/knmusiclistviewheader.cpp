#include "knmusiclistviewheader.h"

KNMusicListViewHeader::KNMusicListViewHeader(QWidget *parent) :
    QHeaderView(Qt::Horizontal, parent)
{
    //Set properties.
    setSectionsMovable(true);
    setDefaultAlignment(Qt::AlignLeft);

    //Set palette.
    QPalette pal=palette();
    pal.setColor(QPalette::Base, QColor(0x30, 0x30, 0x30));
    pal.setColor(QPalette::Window, QColor(0x30, 0x30, 0x30));
    pal.setColor(QPalette::Button, QColor(0x30, 0x30, 0x30));
    pal.setColor(QPalette::ButtonText, QColor(0xff, 0xff, 0xff));
    setPalette(pal);
}

void KNMusicListViewHeader::moveToFirst(int logicalIndex)
{
    moveSection(visualIndex(logicalIndex), 0);
}
