#include <QDebug>

#include <QStyleFactory>
#include <QMouseEvent>
#include <QTimeLine>

#include "../../knlocale.h"

#include "knmusiclistviewheader.h"

#include "knmusiclistview.h"

KNMusicListView::KNMusicListView(QWidget *parent) :
    QTreeView(parent)
{
    //Set properties.
    setStyle(QStyleFactory::create("fusion"));
    setAnimated(true);
    setUniformRowHeights(true);
    setSortingEnabled(true);
    setAlternatingRowColors(true);
    //setVerticalScrollMode(QAbstractItemView::ScrollPerPixel);

    //Set palette.
    int minGrey=0x20;
    m_backgroundColor=QColor(minGrey, minGrey, minGrey);
    m_palette=palette();
    m_palette.setColor(QPalette::Base, m_backgroundColor);
    m_palette.setColor(QPalette::AlternateBase, QColor(0x30, 0x30, 0x30));
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

    m_mouseIn=new QTimeLine(200, this);
    m_mouseIn->setUpdateInterval(2);
    m_mouseIn->setEndFrame(0x40);
    connect(m_mouseIn, &QTimeLine::frameChanged,
            this, &KNMusicListView::changeBackground);

    m_mouseOut=new QTimeLine(200, this);
    m_mouseOut->setUpdateInterval(2);
    m_mouseOut->setEndFrame(minGrey);
    connect(m_mouseOut, &QTimeLine::frameChanged,
            this, &KNMusicListView::changeBackground);
}

void KNMusicListView::resetHeader()
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
    setColumnHidden(KNMusicGlobal::Plays, false);
    m_headerWidget->moveToFirst(KNMusicGlobal::Plays);
    m_headerWidget->moveToFirst(KNMusicGlobal::Rating);
    m_headerWidget->moveToFirst(KNMusicGlobal::Genre);
    m_headerWidget->moveToFirst(KNMusicGlobal::Album);
    m_headerWidget->moveToFirst(KNMusicGlobal::Artist);
    m_headerWidget->moveToFirst(KNMusicGlobal::Time);
    m_headerWidget->moveToFirst(KNMusicGlobal::Name);
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

void KNMusicListView::changeBackground(int frameData)
{
    m_backgroundColor=QColor(frameData, frameData, frameData);
    m_palette.setColor(QPalette::Base, m_backgroundColor);
    m_palette.setColor(QPalette::Button, m_backgroundColor);
    setPalette(m_palette);;
}
