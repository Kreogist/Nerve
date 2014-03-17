#ifndef KNMUSICLISTVIEW_H
#define KNMUSICLISTVIEW_H

#include <QTreeView>

class KNLocale;
class KNMusicListView : public QTreeView
{
    Q_OBJECT
public:
    explicit KNMusicListView(QWidget *parent = 0);

signals:

public slots:
    void retranslate();
    void retranslateAndSet();

private:
    KNLocale *m_locale;

    enum MusicHeader
    {
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
    QString m_headerCaptions[MusicHeaderCount];
};

#endif // KNMUSICLISTVIEW_H
