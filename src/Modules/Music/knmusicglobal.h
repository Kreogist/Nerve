#ifndef KNMUSICGLOBAL_H
#define KNMUSICGLOBAL_H

#include <QObject>
#include <QStringList>

class KNMusicGlobal : public QObject
{
    Q_OBJECT
public:
    static KNMusicGlobal *instance();
    enum MusicDatas
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
        MusicDataCount
    };
    QString getGenre(const int &index);
    QString getGenre(const QString &value);

signals:

public slots:

private:
    static KNMusicGlobal *m_instance;
    KNMusicGlobal();
    QStringList m_genreList;
};

#endif // KNMUSICGLOBAL_H
