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

    struct ID3v2Image
    {
        QString description;
        QImage image;
    };

    explicit KNMusicTagID3v2(QObject *parent = 0);
    QString textData(const int &key) const;
    int id3v2RatingData() const;
    void clearCache();

    bool readTag(const QFile &mediaFile,
                 QDataStream &mediaData);

    QImage tagImage(const int &index) const;
    QImage firstAvaliableImage() const;

signals:

public slots:

private:
    QString id3v2DataToString(const QByteArray &value) const;
    int hexToStarRating(const quint8 &hex) const;
    void processAPIC(const QByteArray &value);
    void processPIC(const QByteArray &value);
    void detectCodec(const QByteArray &value);
    //Version
    int m_version, m_revision;
    //Flags
    bool m_unsynchronisation,
         m_extendedHeader,
         m_experimentalIndicator,
         m_useShortFrames;
    //Frames
    QStringList m_frameID;
    QList<QByteArray> m_frameData;

    QMap<int, ID3v2Image> m_tagImages;
    QTextCodec *m_beCodec, *m_leCodec, *m_isoCodec, *m_localCodec;
    QString m_frames[ID3v2ItemCount][2];
    char *m_rawTagData;
};

#endif // KNMUSICTAGID3V2_H
