#include <QHeaderView>

#include "../knmusicglobal.h"

#include "knmusicgenresongs.h"

KNMusicGenreSongs::KNMusicGenreSongs(QWidget *parent) :
    KNMusicListView(parent)
{
    ;
}

void KNMusicGenreSongs::resetHeader()
{
    connect(header(), &QHeaderView::sortIndicatorChanged,
            this, &KNMusicGenreSongs::onActionSort);
    for(int i=KNMusicGlobal::Name+1;
        i<KNMusicGlobal::MusicDataCount;
        i++)
    {
        setColumnHidden(i, true);
    }
    setColumnHidden(KNMusicGlobal::Time, false);
    setColumnHidden(KNMusicGlobal::Album, false);
    setColumnHidden(KNMusicGlobal::Artist, false);
    setColumnHidden(KNMusicGlobal::Rating, false);
    moveToFirst(KNMusicGlobal::Rating);
    moveToFirst(KNMusicGlobal::Album);
    moveToFirst(KNMusicGlobal::Artist);
    moveToFirst(KNMusicGlobal::Time);
    moveToFirst(KNMusicGlobal::Name);
}
