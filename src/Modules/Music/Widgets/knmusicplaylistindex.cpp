#include <QPainter>

#include "knmusicplaylistindex.h"

KNMusicPlaylistIndex::KNMusicPlaylistIndex(QObject *parent) :
    QStyledItemDelegate(parent)
{
}

void KNMusicPlaylistIndex::paint(QPainter *painter,
                                 const QStyleOptionViewItem &option,
                                 const QModelIndex &index) const
{
    QStyledItemDelegate::paint(painter, option, index);
    painter->drawText(option.rect.x(),
                      option.rect.y(),
                      option.rect.width()-10,
                      option.rect.height(),
                      Qt::AlignRight | Qt::AlignVCenter,
                      QString::number(index.row()+1));
}

QSize KNMusicPlaylistIndex::sizeHint(const QStyleOptionViewItem &option,
                                     const QModelIndex &index) const
{
    QString text=QString::number(index.row()+1);
    return QSize(option.fontMetrics.width(text)+20,
                 option.fontMetrics.height());
}
