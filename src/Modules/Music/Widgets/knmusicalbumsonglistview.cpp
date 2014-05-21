#include <QHeaderView>

#include <QDebug>

#include "../knmusicglobal.h"

#include "knmusicalbumsonglistview.h"

KNMusicAlbumSongListView::KNMusicAlbumSongListView(QWidget *parent) :
    KNMusicListViewBase(parent)
{
    ;
}

void KNMusicAlbumSongListView::resetHeader()
{
    connect(header(), &QHeaderView::sortIndicatorChanged,
            this, &KNMusicAlbumSongListView::onActionSort);
    for(int i=KNMusicGlobal::Name+1;
        i<KNMusicGlobal::MusicDataCount;
        i++)
    {
        setColumnHidden(i, true);
    }
    setColumnHidden(KNMusicGlobal::Time, false);
    setColumnHidden(KNMusicGlobal::Artist, false);
    setColumnHidden(KNMusicGlobal::Genre, false);
    setColumnHidden(KNMusicGlobal::Rating, false);
    setColumnHidden(KNMusicGlobal::TrackNumber, false);
    moveToFirst(KNMusicGlobal::Rating);
    moveToFirst(KNMusicGlobal::Genre);
    moveToFirst(KNMusicGlobal::Artist);
    moveToFirst(KNMusicGlobal::Time);
    moveToFirst(KNMusicGlobal::Name);
    moveToFirst(KNMusicGlobal::TrackNumber);
}

void KNMusicAlbumSongListView::resizeHeader()
{
    ;
}
