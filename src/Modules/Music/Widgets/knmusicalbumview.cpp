#include <QDebug>

#include <QVariant>
#include <QIcon>
#include <QPainter>
#include <QScrollBar>

#include "knmusicalbumview.h"

KNMusicAlbumDelegate::KNMusicAlbumDelegate(QObject *parent) :
    QStyledItemDelegate(parent)
{
    ;
}

void KNMusicAlbumDelegate::paint(QPainter *painter,
                                 const QStyleOptionViewItem &option,
                                 const QModelIndex &index) const
{
    //painter->drawText(option.rect, index.data().toString());
    QIcon currentIcon=qvariant_cast<QIcon>(index.data(Qt::DecorationRole));
    painter->drawPixmap(option.rect, currentIcon.pixmap(255,255));
    //QStyledItemDelegate::paint(painter, option, index);
}

KNMusicAlbumView::KNMusicAlbumView(QWidget *parent) :
    QListView(parent)
{
    setViewMode(IconMode);
    setIconSize(QSize(128, 128));
    setGridSize(QSize(185, 200));
    setMovement(Static);
    setItemDelegate(new KNMusicAlbumDelegate(this));
    setUniformItemSizes(true);
    setResizeMode(Adjust);
    setLayoutMode(QListView::Batched);
    setVerticalScrollMode(QAbstractItemView::ScrollPerPixel);
    setWrapping(true);
}
