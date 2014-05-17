#ifndef KNMUSICTAGWMA_H
#define KNMUSICTAGWMA_H

#include <QByteArray>
#include <QMap>

#include "knmusictagbase.h"

class QTextCodec;
class KNMusicTagWma : public KNMusicTagBase
{
    Q_OBJECT
public:
    enum WMAItems
    {
        Artist,
        Copyright,
        Description,
        Rating,
        Name,
        AlbumArtist,
        Album,
        BeatsPerMinuate,
        Comments,
        Composer,
        Genre,
        Year,
        TrackNumber,
        WMAItemsCount
    };

    enum WMA_FRAMEID_T
    {
        WMA_FRAMEID_AUTHOR = 0,
        WMA_FRAMEID_COPYRIGHT,
        WMA_FRAMEID_DESCRIPTION,
        WMA_FRAMEID_RATING,
        WMA_FRAMEID_TITLE,
        WMAFrameIDCount
    };
    explicit KNMusicTagWma(QObject *parent = 0);

    void clearCache();
    bool readTag(const QString &filePath);
    QString tagStringData(const QString &frameKey) const;
    QString standardTag(const int &index) const;

signals:

public slots:

private:
    unsigned char m_headerMark[17]={0x30, 0x26, 0xB2, 0x75,
                                    0x8E, 0x66, 0xCF, 0x11,
                                    0xA6, 0xD9, 0x00, 0xAA,
                                    0x00, 0x62, 0xCE, 0x6C};
    unsigned char m_standardFrame[17]={0x33, 0x26, 0xB2, 0x75,
                                       0x8E, 0x66, 0xCF, 0x11,
                                       0xA6, 0xD9, 0x00, 0xAA,
                                       0x00, 0x62, 0xCE, 0x6C};
    unsigned char m_extendedFrame[17]={0x40, 0xA4, 0xD0, 0xD2,
                                       0x07, 0xE3, 0xD2, 0x11,
                                       0x97, 0xF0, 0x00, 0xA0,
                                       0xC9, 0x5E, 0xA8, 0x50};

    enum WMAStandardFrame
    {
        Title=0,
        Author,
        Copyright,
        Description,
        Rating
    };

    QByteArray m_wmaTags[WMAFrameIDCount];
    QTextCodec *m_utf16Codec;
    QMap<QString, QByteArray> m_frameDatas;
    QString m_frames[WMAItemsCount];
};

#endif // KNMUSICTAGWMA_H
