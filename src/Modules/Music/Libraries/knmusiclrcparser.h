#ifndef KNMUSICLRCPARSER_H
#define KNMUSICLRCPARSER_H

#include <QList>
#include <QStringList>
#include <QRegularExpression>

#include "knmusiclrcparserbase.h"

class KNMusicLRCParser : public KNMusicLRCParserBase
{
    Q_OBJECT
public:
    explicit KNMusicLRCParser(QObject *parent = 0);
    bool readLyrics(QString filePath);
    QString title() const;
    QString artist() const;
    QString album() const;
    QList<float> positions() const;
    QStringList lyricsTexts() const;

signals:

public slots:

private:
    void resetCache();
    struct lrcFrame
    {
        float position;
        QString text;
    };
    enum headerType
    {
        TitleFrame,
        ArtistFrame,
        AlbumFrame,
        ByFrame
    };

    void parseLyrics();
    void parseFrame(const QString &frame, const QString &lineData);
    static bool frameLessThan(const lrcFrame &frameLeft, const lrcFrame &frameRight);
    QStringList m_lyricsRawData,
                m_headerText;
    QList<lrcFrame> m_lyrics;
    QList<float> m_position;
    QStringList m_lyricsText;
    QString m_title, m_artist, m_album;
    QRegularExpression m_frameCatch;
};

#endif // KNMUSICLRCPARSER_H
