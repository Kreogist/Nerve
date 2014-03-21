#ifndef KNMUSICGLOBAL_H
#define KNMUSICGLOBAL_H

#include <QObject>
#include <QDateTime>
#include <QPixmap>
#include <QMetaType>
#include <QStringList>

class KNMusicGlobal : public QObject
{
    Q_OBJECT
public:
    static KNMusicGlobal *instance();
    struct MusicDetailsInfo
    {
        quint64 size;
        QDateTime dateModified;
        QDateTime lastPlayed;
        QDateTime dateAdded;
        QPixmap coverImage;
    };

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
        Rating,
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
