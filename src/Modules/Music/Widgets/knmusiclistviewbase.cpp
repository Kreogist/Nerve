#include <QHeaderView>
#include <QScrollBar>

#include "knmusicratingdelegate.h"
#include "../knmusicglobal.h"

#include "knmusiclistviewbase.h"

KNMusicListViewBase::KNMusicListViewBase(QWidget *parent) :
    QTreeView(parent)
{
    //Set properties.
    setIndentation(0);
    setMouseTracking(true);
    setUniformRowHeights(true);
    setSortingEnabled(true);
    setDragDropMode(QAbstractItemView::DragOnly);
    setAllColumnsShowFocus(true);
    setAlternatingRowColors(true);
    setSizeAdjustPolicy(QAbstractScrollArea::AdjustToContents);
    setVerticalScrollMode(QAbstractItemView::ScrollPerPixel);
    setSelectionBehavior(QAbstractItemView::SelectRows);
    setSelectionMode(QAbstractItemView::ContiguousSelection);

    horizontalScrollBar()->setSingleStep(5);
    horizontalScrollBar()->setPageStep(5);
    verticalScrollBar()->setSingleStep(4);
    verticalScrollBar()->setPageStep(4);

    setItemDelegateForColumn(KNMusicGlobal::Rating,
                             new KNMusicRatingDelegate(this));
    setEditTriggers(QAbstractItemView::DoubleClicked);
}

void KNMusicListViewBase::resetHeader()
{
    for(int i=KNMusicGlobal::Name+1;
        i<KNMusicGlobal::MusicDataCount;
        i++)
    {
        setColumnHidden(i, true);
    }
    setColumnHidden(KNMusicGlobal::Time, false);
    setColumnHidden(KNMusicGlobal::Artist, false);
    setColumnHidden(KNMusicGlobal::Album, false);
    setColumnHidden(KNMusicGlobal::Genre, false);
    setColumnHidden(KNMusicGlobal::Rating, false);
    moveToFirst(KNMusicGlobal::Rating);
    moveToFirst(KNMusicGlobal::Genre);
    moveToFirst(KNMusicGlobal::Album);
    moveToFirst(KNMusicGlobal::Artist);
    moveToFirst(KNMusicGlobal::Time);
    moveToFirst(KNMusicGlobal::Name);
}

void KNMusicListViewBase::moveToFirst(const int &logicalHeaderIndex)
{
    header()->moveSection(header()->visualIndex(logicalHeaderIndex), 0);
}

void KNMusicListViewBase::retranslate()
{
    ;
}

void KNMusicListViewBase::retranslateAndSet()
{
    ;
}

void KNMusicListViewBase::setHeaderAlignment(const int &logicalHeaderIndex,
                                             Qt::Alignment alignment)
{
    model()->setHeaderData(logicalHeaderIndex,
                           Qt::Horizontal,
                           QVariant(alignment),
                           Qt::TextAlignmentRole);
}

void KNMusicListViewBase::onActionSort(int logicalIndex, Qt::SortOrder order)
{
    Q_UNUSED(logicalIndex);
    Q_UNUSED(order);
    if(currentIndex().isValid())
    {
        scrollTo(currentIndex());
    }
}
