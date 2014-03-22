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
    QPalette pal=palette();
    pal.setColor(QPalette::Base, QColor(0x20, 0x20, 0x20));
    pal.setColor(QPalette::AlternateBase, QColor(0x30, 0x30, 0x30));
    pal.setColor(QPalette::Window, QColor(0x20, 0x20, 0x20));
    pal.setColor(QPalette::Button, QColor(0x20, 0x20, 0x20));
    pal.setColor(QPalette::Text, QColor(0xff, 0xff, 0xff));
    pal.setColor(QPalette::Highlight, QColor(0x60, 0x60, 0x60));
    pal.setColor(QPalette::HighlightedText, QColor(0xf7, 0xcf, 0x3d));
    setPalette(pal);

    //Set locale.
    retranslate();
    connect(KNLocale::instance(), SIGNAL(languageChanged()),
            this, SLOT(retranslateAndSet()));

    //Set music header.
    m_headerWidget=new KNMusicListViewHeader(this);
    setHeader(m_headerWidget);
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
