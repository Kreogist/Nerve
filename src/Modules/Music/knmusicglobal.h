#ifndef KNMUSICGLOBAL_H
#define KNMUSICGLOBAL_H

#include <QObject>
#include <QDateTime>
#include <QPixmap>
#include <QMetaType>
#include <QStringList>
#include <QMap>

class KNMusicGlobal : public QObject
{
    Q_OBJECT
public:
    static KNMusicGlobal *instance();
    struct MusicDetailsInfo
    {
        quint64 size;
        QString filePath;
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
        Time,
        TrackNumber,
        Year,
        MusicDataCount
    };
    QString getGenre(const int &index) const;
    QString getGenre(const QString &value) const;
    QPixmap getGenreImage(const int &index) const;
    QPixmap getGenreImage(const QString &value) const;
    QString getHeader(const int &index);

signals:

public slots:
    void retranslate();
    void retranslateAndSet();

private:
    static KNMusicGlobal *m_instance;
    KNMusicGlobal();
    QStringList m_genreList;
    QString m_header[MusicDataCount];
    QMap<QString, QString> m_genreImage;
};

#endif // KNMUSICGLOBAL_H
