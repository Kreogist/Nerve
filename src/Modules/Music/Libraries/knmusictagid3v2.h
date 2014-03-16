#ifndef KNMUSICTAGID3V2_H
#define KNMUSICTAGID3V2_H

#include <QMap>
#include <QPixmap>

#include "knmusictagbase.h"

class QTextCodec;
class KNMusicTagID3v2 : public KNMusicTagBase
{
    Q_OBJECT
public:
    struct IDv2Frame
    {
        QString frameID;
        QByteArray data;
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
        QList<IDv2Frame> frames;
    };

    struct ID3v2Image
    {
        QString description;
        QPixmap image;
    };

    QString id3v2String(const QByteArray &value);

    explicit KNMusicTagID3v2(QObject *parent = 0);
    bool readTag(const QString &filePath);

    QMap<int, ID3v2Image> tagImages() const;
    void setTagImages(const QMap<int, ID3v2Image> &tagImages);

signals:

public slots:

private:
    void processAPIC(const QByteArray &value);
    QMap<int, ID3v2Image> m_tagImages;
    ID3v2Data m_tagData;
    QTextCodec *m_beCodec, *m_leCodec;
};

#endif // KNMUSICTAGID3V2_H
