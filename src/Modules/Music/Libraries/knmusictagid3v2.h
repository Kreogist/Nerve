#ifndef KNMUSICTAGID3V2_H
#define KNMUSICTAGID3V2_H

#include <QMap>
#include <QList>
#include <QByteArray>
#include <QPixmap>

#include "knmusictagbase.h"

class QTextCodec;
class KNMusicTagID3v2 : public KNMusicTagBase
{
    Q_OBJECT
public:
    enum ID3v2Items
    {
        Name,
        Artist,
        Album,
        AlbumArtist,
        BeatsPerMinuate,
        Category,
        Composer,
        Description,
        Genre,
        Track,
        Disc,
        Rating,
        Year,
        ID3v2ItemCount
    };

    struct ID3v2Data
    {
        //Version
        int version;
        int revision;
        //Flags
        bool unsynchronisation;
        bool extendedHeader;
        bool experimentalIndicator;
        //Frames
        QStringList frameID;
        QList<QByteArray> frameData;
    };

    struct ID3v2Image
    {
        QString description;
        QImage image;
    };

    QString textData(const int &key) const;
    explicit KNMusicTagID3v2(QObject *parent = 0);
    QString id3v2String(const QString &frameID) const;
    int id3v2RatingData() const;
    QByteArray id3v2Raw(const QString &frameID) const;
    void clearCache();
    int version();

    bool readTag(const QString &filePath);

    QImage tagImage(const int &index) const;
    QImage firstAvaliableImage() const;

signals:

public slots:

private:
    QString id3v2DataToString(const QByteArray &value) const;
    int hexToStarRating(const quint8 &hex) const;
    void processAPIC(const QByteArray &value);
    void processPIC(const QByteArray &value);
    QMap<int, ID3v2Image> m_tagImages;
    ID3v2Data m_tagData;
    bool m_useShortFrames;
    QTextCodec *m_beCodec, *m_leCodec, *m_utf8Codec, *m_localeCodec;
    QString m_frames[ID3v2ItemCount][2];
};

#endif // KNMUSICTAGID3V2_H
