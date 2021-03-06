#ifndef KNMUSICGLOBAL_H
#define KNMUSICGLOBAL_H

#include <QObject>
#include <QDateTime>
#include <QPixmap>
#include <QMetaType>
#include <QModelIndexList>
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
        int rating=0;
        int duration=0;
        float bitRate=0.0;
        float samplingRate=0.0;
        QDateTime dateModified;
        QDateTime lastPlayed;
        QImage coverImage;
        QString filePath;
        QDateTime dateAdded;
        QString coverImageHash;
    };

    enum MusicModelProperty
    {
        FilePathRole=Qt::UserRole,
        FileNameRole,
        ArtworkKeyRole,
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
        DiscCount,
        DiscNumber,
        Equalizer,
        Genre,
        Grouping,
        Kind,
        LastPlayed,
        Rating,
        SampleRate,
        Size,
        Time,
        TrackCount,
        TrackNumber,
        Year,
        MusicDataCount
    };

    enum MusicCategory
    {
        SongsView,
        ArtistView,
        AlbumView,
        GenreView
    };

    enum LoopMode
    {
        NoRepeat,
        RepeatAll,
        RepeatSong,
        LoopCount
    };

    QString getGenre(const int &index) const;
    QString getGenre(const QString &value) const;
    QPixmap getGenreImage(const int &index) const;
    QPixmap getGenreImage(const QString &value) const;
    QString getHeader(const int &index);
    int getMusicType(const QString &suffix) const;
    QString getDescription(const int &type) const;
    QImage noAlbumImage() const;
    QStringList genreList() const;
    int genreListSize() const;
    void setSelectedIndexes(const QModelIndexList &indexes);
    QModelIndexList selectedIndexes() const;
    int genreIndex(const QString &genre) const;
    int selectedColumn() const;
    void setSelectedColumn(int selectedColumn);

signals:

public slots:
    void retranslate();
    void retranslateAndSet();

private:
    static KNMusicGlobal *m_instance;
    KNMusicGlobal();
    QStringList m_genreList;
    QStringList m_suffixs;
    QStringList m_suffixDescription;
    QString m_header[MusicDataCount];
    QImage m_noAlbumImage;
    QMap<QString, QString> m_genreImage;
    QModelIndexList m_currentIndexes;
    int m_selectedColumn=-1;
};

#endif // KNMUSICGLOBAL_H
