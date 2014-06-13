#include <QPainter>
#include <QFontMetrics>
#include <QTextOption>

#include <QDebug>

#include "knmusicresultview.h"

KNMusicResultItemDelegate::KNMusicResultItemDelegate(QObject *parent) :
    QStyledItemDelegate(parent)
{
    ;
}

void KNMusicResultItemDelegate::paint(QPainter *painter,
                                      const QStyleOptionViewItem &option,
                                      const QModelIndex &index) const
{
    QStyleOptionViewItemV4 options=option;
    initStyleOption(&options, index);

    QString displayText=options.fontMetrics.elidedText(options.text,
                                                       options.textElideMode,
                                                       (options.rect.width()<<1)-10);
    options.text.clear();
    options.widget->style()->drawControl(QStyle::CE_ItemViewItem, &options, painter);
    QTextOption displayOption;
    displayOption.setAlignment(Qt::AlignLeft | Qt::AlignVCenter);
    displayOption.setWrapMode(QTextOption::WrapAnywhere);
    painter->drawText(QRectF(options.rect),
                      displayText,
                      displayOption);
}

QSize KNMusicResultItemDelegate::sizeHint(const QStyleOptionViewItem &option,
                                          const QModelIndex &index) const
{
    QSize originalSize=QStyledItemDelegate::sizeHint(option, index);
    return QSize(originalSize.width(), originalSize.height()<<1);
}

KNMusicResultView::KNMusicResultView(QWidget *parent) :
    QTreeView(parent)
{
    //Set properties.
    setAutoFillBackground(false);
    setSelectionMode(QAbstractItemView::SingleSelection);
    setSelectionBehavior(QAbstractItemView::SelectRows);
    setVerticalScrollMode(QAbstractItemView::ScrollPerPixel);

    //Set palette.
    QPalette pal=palette();
    pal.setColor(QPalette::Window, QColor(0,0,0,0));
    pal.setColor(QPalette::Base, QColor(0,0,0,0));
    setPalette(pal);

    KNMusicResultItemDelegate *delegate=new KNMusicResultItemDelegate(this);
    setItemDelegateForColumn(1, delegate);
    setItemDelegateForColumn(2, delegate);
}

