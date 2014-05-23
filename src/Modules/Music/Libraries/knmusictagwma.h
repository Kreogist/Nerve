#ifndef KNMUSICTAGWMA_H
#define KNMUSICTAGWMA_H

#include <QByteArray>
#include <QMap>
#include <QImage>

#include "knmusictagbase.h"

class QTextCodec;
class KNMusicTagWMA : public KNMusicTagBase
{
    Q_OBJECT
public:
    enum WMAItems
    {
        Name,
        Artist,
        Copyright,
        Description,
        Rating,
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
    explicit KNMusicTagWMA(QObject *parent = 0);

    void clearCache();
    bool readTag(const QFile &mediaFile,
                 QDataStream &mediaData);
    QString textData(const int &key) const;
    QImage albumArt() const;

signals:

public slots:

private:
    void processPicture();
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

    QTextCodec *m_utf16leCodec, *m_utf16beCodec;
    QMap<QString, QByteArray> m_frameDatas;
    QString m_frames[WMAItemsCount], m_albumArtDescription;
    QImage m_albumArt;
};

#endif // KNMUSICTAGWMA_H
