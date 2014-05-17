#include <QHeaderView>

#include "../knmusicglobal.h"

#include "knmusicartistsongs.h"

KNMusicArtistSongs::KNMusicArtistSongs(QWidget *parent) :
    KNMusicListView(parent)
{
    ;
}

void KNMusicArtistSongs::resetHeader()
{
    connect(header(), &QHeaderView::sortIndicatorChanged,
            this, &KNMusicArtistSongs::onActionSort);
    for(int i=KNMusicGlobal::Name+1;
        i<KNMusicGlobal::MusicDataCount;
        i++)
    {
        setColumnHidden(i, true);
    }
    setColumnHidden(KNMusicGlobal::Time, false);
    setColumnHidden(KNMusicGlobal::Album, false);
    setColumnHidden(KNMusicGlobal::Genre, false);
    setColumnHidden(KNMusicGlobal::Rating, false);
    moveToFirst(KNMusicGlobal::Rating);
    moveToFirst(KNMusicGlobal::Genre);
    moveToFirst(KNMusicGlobal::Album);
    moveToFirst(KNMusicGlobal::Time);
    moveToFirst(KNMusicGlobal::Name);
}
