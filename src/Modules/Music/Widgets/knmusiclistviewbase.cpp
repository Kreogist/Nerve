#include "knmusiclistviewbase.h"

#include <QScrollBar>
#include <QMouseEvent>
#include <QToolTip>
#include <QHelpEvent>

#include <QDebug>

#include "../../knlocale.h"

#include "knmusiclistviewheader.h"
#include "knmusicratingdelegate.h"
#include "knmusicdetailtooltip.h"

KNMusicListViewBase::KNMusicListViewBase(QWidget *parent) :
    QTreeView(parent)
{
    //Set properties.
    setIndentation(0);
    setMouseTracking(true);
    setUniformRowHeights(true);
    setSortingEnabled(true);
    setAllColumnsShowFocus(true);
    setAlternatingRowColors(true);
    //setSelectionMode(QAbstractItemView::ExtendedSelection);
    setSizeAdjustPolicy(QAbstractScrollArea::AdjustToContents);
    setVerticalScrollMode(QAbstractItemView::ScrollPerPixel);

    horizontalScrollBar()->setSingleStep(5);
    horizontalScrollBar()->setPageStep(5);
    verticalScrollBar()->setSingleStep(4);
    verticalScrollBar()->setPageStep(4);

    //Set locale.
    retranslate();
    connect(KNLocale::instance(), &KNLocale::languageChanged,
            this, &KNMusicListViewBase::retranslateAndSet);

    //Set music header.
    m_headerWidget=new KNMusicListViewHeader(this);
    setHeader(m_headerWidget);
    connect(m_headerWidget, &KNMusicListViewHeader::requireChangeVisible,
            this, &KNMusicListViewBase::onSectionVisibleChanged);

    m_musicDetailTooltip=new KNMusicDetailTooltip(this);
    connect(this, &KNMusicListViewBase::activated,
            this, &KNMusicListViewBase::onItemActived);
}

void KNMusicListViewBase::resetHeader()
{
    setItemDelegateForColumn(KNMusicGlobal::Rating,
                             new KNMusicRatingDelegate(this));
    setEditTriggers(QAbstractItemView::SelectedClicked);
    connect(header(), &QHeaderView::sortIndicatorChanged,
            this, &KNMusicListViewBase::onActionSort);
    /*for(int i=KNMusicGlobal::Name+1;
        i<KNMusicGlobal::MusicDataCount;
        i++)
    {
        setColumnHidden(i, true);
    }
    setColumnHidden(KNMusicGlobal::Time, false);
    setColumnHidden(KNMusicGlobal::Artist, false);
    setColumnHidden(KNMusicGlobal::Album, false);
    setColumnHidden(KNMusicGlobal::Genre, false);
    setColumnHidden(KNMusicGlobal::Rating, false);*/
    moveToFirst(KNMusicGlobal::Rating);
    moveToFirst(KNMusicGlobal::Genre);
    moveToFirst(KNMusicGlobal::Album);
    moveToFirst(KNMusicGlobal::Artist);
    moveToFirst(KNMusicGlobal::Time);
    moveToFirst(KNMusicGlobal::Name);
}

void KNMusicListViewBase::moveToFirst(const int &logicalHeaderIndex)
{
    m_headerWidget->moveToFirst(logicalHeaderIndex);
}

void KNMusicListViewBase::setHeaderAlignment(const int &logicalHeaderIndex,
                                             Qt::Alignment alignment)
{
    model()->setHeaderData(logicalHeaderIndex,
                           Qt::Horizontal,
                           QVariant(alignment),
                           Qt::TextAlignmentRole);
}

void KNMusicListViewBase::retranslate()
{
    ;
}

void KNMusicListViewBase::retranslateAndSet()
{
    retranslate();
}

void KNMusicListViewBase::onActionSort(int logicalIndex, Qt::SortOrder order)
{
    Q_UNUSED(logicalIndex);
    Q_UNUSED(order);
    if(currentIndex().isValid())
    {
        scrollTo(currentIndex(), PositionAtCenter);
    }
}

void KNMusicListViewBase::mouseReleaseEvent(QMouseEvent *event)
{
    QTreeView::mouseReleaseEvent(event);
    if(event->button()==Qt::RightButton &&
          rect().contains(event->pos()))
    {
        QModelIndex posTest=indexAt(event->pos());
        if(posTest.isValid())
        {
            emit requireShowContextMenu(event->globalPos(),
                                        posTest);
        }
    }
}

void KNMusicListViewBase::leaveEvent(QEvent *event)
{
    //m_musicDetailTooltip->hide();
    QTreeView::leaveEvent(event);
}

void KNMusicListViewBase::onSectionVisibleChanged(const int &index,
                                              const bool &visible)
{
    setColumnHidden(index, !visible);
}

void KNMusicListViewBase::onItemActived(const QModelIndex &index)
{
    if(index.isValid())
    {
        emit requireOpenUrl(index);
    }
}
