#ifndef KNMUSICGLOBAL_H
#define KNMUSICGLOBAL_H

#include <QObject>

class KNMusicGlobal : public QObject
{
    Q_OBJECT
public:
    static KNMusicGlobal *instance();
    enum MusicHeader
    {
        Name,
        Album,
        AlbumArtist,
        AlbumRating,
        Artist,
        BeatsPerMinuate,
        BitRate,
        Category,
        Comments,
        Composer,
        DateAdded,
        DateModified,
        Description,
        DiscNumber,
        EpisodeID,
        EpisodeNumber,
        Equalizer,
        Genre,
        Grouping,
        Kind,
        LastPlayed,
        LastSkipped,
        Plays,
        PurchaseDate,
        Rating,
        ReleaseData,
        SampleRate,
        Season,
        Show,
        Size,
        Skips,
        Time,
        TrackNumber,
        Year,
        MusicHeaderCount
    };

signals:

public slots:

private:
    static KNMusicGlobal *m_instance;
    KNMusicGlobal();

};

#endif // KNMUSICGLOBAL_H
