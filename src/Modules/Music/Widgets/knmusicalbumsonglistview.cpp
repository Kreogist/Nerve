#include <QHeaderView>

#include <QDebug>

#include "../knmusicglobal.h"

#include "knmusicalbumsonglistview.h"

KNMusicAlbumSongListView::KNMusicAlbumSongListView(QWidget *parent) :
    KNMusicLibraryListview(parent)
{
    QPalette pal=palette();
    pal.setColor(QPalette::Base, QColor(0,0,0,0));
    setPalette(pal);
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
    int maximumTrack=0, maximumDuration=0;
    for(int i=0; i<model()->rowCount(); i++)
    {
        QString currentTrackString=model()->data(model()->index(i,
                                                                KNMusicGlobal::TrackNumber),
                                                 Qt::DisplayRole).toString(),
                currentDuration=model()->data(model()->index(i,
                                                             KNMusicGlobal::Time),
                                              Qt::DisplayRole).toString();
        int currentTrackWidth=fontMetrics().width(currentTrackString),
            currentDurationWidth=fontMetrics().width(currentDuration);
        maximumTrack=maximumTrack<currentTrackWidth?currentTrackWidth:maximumTrack;
        maximumDuration=maximumDuration<currentDurationWidth?currentDurationWidth:maximumDuration;
    }
    setColumnWidth(KNMusicGlobal::TrackNumber, maximumTrack+(maximumTrack>>1));
    setColumnWidth(KNMusicGlobal::Time, maximumDuration+(maximumDuration>>2));
    resizeColumnToContents(KNMusicGlobal::Name);
}
