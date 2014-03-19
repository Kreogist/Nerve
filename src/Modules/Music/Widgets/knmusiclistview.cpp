#include <QDebug>

#include <QStyleFactory>

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
    setVerticalScrollMode(QAbstractItemView::ScrollPerPixel);

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
    setColumnHidden(KNMusicGlobal::Rating, false);
    setColumnHidden(KNMusicGlobal::Plays, false);*/
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
