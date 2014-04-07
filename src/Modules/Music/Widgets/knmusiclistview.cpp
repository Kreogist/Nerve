#include <QDebug>

#include <QStyleFactory>
#include <QMouseEvent>
#include <QTimeLine>
#include <QToolTip>
#include <QScrollBar>
#include <QHelpEvent>

#include "../../knlocale.h"

#include "knmusiclistviewheader.h"
#include "knmusicratingdelegate.h"
#include "knmusicdetailtooltip.h"

#include "knmusiclistview.h"

KNMusicListView::KNMusicListView(QWidget *parent) :
    QTreeView(parent)
{
    //Set properties.
    setMouseTracking(true);
    setUniformRowHeights(true);
    setSortingEnabled(true);
    setAlternatingRowColors(true);
    setSizeAdjustPolicy(QAbstractScrollArea::AdjustToContents);
    setVerticalScrollMode(QAbstractItemView::ScrollPerPixel);
    setAllColumnsShowFocus(true);

    horizontalScrollBar()->setSingleStep(5);
    verticalScrollBar()->setSingleStep(4);

    //Set palette.
    int minGrey=0x10;
    m_backgroundColor=QColor(minGrey, minGrey, minGrey);
    m_palette=palette();
    m_palette.setColor(QPalette::Base, m_backgroundColor);
    m_palette.setColor(QPalette::AlternateBase, QColor(0x20, 0x20, 0x20));
    m_palette.setColor(QPalette::Window, QColor(0x20, 0x20, 0x20));
    m_palette.setColor(QPalette::Button, QColor(0x20, 0x20, 0x20));
    m_palette.setColor(QPalette::Text, QColor(0xff, 0xff, 0xff));
    m_palette.setColor(QPalette::Highlight, QColor(0x60, 0x60, 0x60));
    m_palette.setColor(QPalette::HighlightedText, QColor(0xf7, 0xcf, 0x3d));
    setPalette(m_palette);

    //Set locale.
    retranslate();
    connect(KNLocale::instance(), SIGNAL(languageChanged()),
            this, SLOT(retranslateAndSet()));

    //Set music header.
    m_headerWidget=new KNMusicListViewHeader(this);
    setHeader(m_headerWidget);
    connect(m_headerWidget, &KNMusicListViewHeader::requireChangeVisible,
            this, &KNMusicListView::onSectionVisibleChanged);

    m_musicDetailTooltip=new KNMusicDetailTooltip(this);

    connect(this, &KNMusicListView::doubleClicked,
            this, &KNMusicListView::onItemActived);
    connect(this, &KNMusicListView::activated,
            this, &KNMusicListView::onItemActived);

    m_mouseIn=new QTimeLine(200, this);
    m_mouseIn->setUpdateInterval(5);
    m_mouseIn->setEndFrame(0x30);
    connect(m_mouseIn, &QTimeLine::frameChanged,
            this, &KNMusicListView::changeBackground);

    m_mouseOut=new QTimeLine(200, this);
    m_mouseOut->setUpdateInterval(5);
    m_mouseOut->setEndFrame(minGrey);
    connect(m_mouseOut, &QTimeLine::frameChanged,
            this, &KNMusicListView::changeBackground);
}

void KNMusicListView::resetHeader()
{
    setItemDelegateForColumn(KNMusicGlobal::Rating,
                             new KNMusicRatingDelegate(this));
    setEditTriggers(QAbstractItemView::SelectedClicked);
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

void KNMusicListView::moveToFirst(const int &logicalHeaderIndex)
{
    m_headerWidget->moveToFirst(logicalHeaderIndex);
}

void KNMusicListView::retranslate()
{
    ;
}

void KNMusicListView::retranslateAndSet()
{
    retranslate();
}

void KNMusicListView::mouseReleaseEvent(QMouseEvent *event)
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

void KNMusicListView::enterEvent(QEvent *e)
{
    m_mouseOut->stop();
    m_mouseIn->stop();
    m_mouseIn->setStartFrame(m_backgroundColor.red());
    m_mouseIn->start();
    QTreeView::enterEvent(e);
}

void KNMusicListView::leaveEvent(QEvent *e)
{
    m_mouseIn->stop();
    m_mouseOut->stop();
    m_mouseOut->setStartFrame(m_backgroundColor.red());
    m_mouseOut->start();
    QTreeView::leaveEvent(e);
}

bool KNMusicListView::event(QEvent *event)
{
    if(event->type()==QEvent::ToolTip)
    {
        QHelpEvent *helpEvent=static_cast<QHelpEvent *>(event);
        QModelIndex currentSong=indexAt(helpEvent->pos());
        if(currentSong.isValid())
        {
            if(m_detailIndex!=currentSong)
            {
                m_detailIndex=currentSong;
                //m_musicDetailTooltip->move(helpEvent->globalPos());
                //m_musicDetailTooltip->show();
            }
        }
        else
        {
            //QToolTip::hideText();
            //m_musicDetailTooltip->hide();
            event->ignore();
        }
        return true;
    }
    return QTreeView::event(event);
}

void KNMusicListView::changeBackground(int frameData)
{
    m_backgroundColor=QColor(frameData, frameData, frameData);
    m_palette.setColor(QPalette::AlternateBase, m_backgroundColor);
    m_palette.setColor(QPalette::Button, m_backgroundColor);
    int textParam=(frameData<<1)+127;
    m_palette.setColor(QPalette::Text, QColor(textParam,
                                              textParam,
                                              textParam));
    setPalette(m_palette);
}

void KNMusicListView::onSectionVisibleChanged(const int &index,
                                              const bool &visible)
{
    setColumnHidden(index, !visible);
}

void KNMusicListView::onItemActived(const QModelIndex &index)
{
    if(index.isValid())
    {
        emit requireOpenUrl(index);
    }
}
