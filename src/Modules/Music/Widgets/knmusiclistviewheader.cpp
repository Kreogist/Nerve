#include <QDebug>

#include "knmusiclistviewheadermenu.h"

#include "knmusiclistviewheader.h"

KNMusicListViewHeader::KNMusicListViewHeader(QWidget *parent) :
    KNMusicListViewHeaderBase(parent)
{
    //Set menu.
    m_viewerMenu=new KNMusicListViewHeaderMenu(this);
    connect(this, &KNMusicListViewHeader::customContextMenuRequested,
            this, &KNMusicListViewHeader::showContextMenu);
    connect(m_viewerMenu, &KNMusicListViewHeaderMenu::requireChangeVisible,
            this, &KNMusicListViewHeader::requireChangeVisible);
}

void KNMusicListViewHeader::showContextMenu(const QPoint &mousePoint)
{
    for(int i=0; i<KNMusicGlobal::MusicDataCount; i++)
    {
        m_viewerMenu->setHeaderValue(i, !isSectionHidden(i));
    }
    m_viewerMenu->exec(mapToGlobal(mousePoint));
}
