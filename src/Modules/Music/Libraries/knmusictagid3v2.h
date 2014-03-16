#ifndef KNMUSICTAGID3V2_H
#define KNMUSICTAGID3V2_H

#include <QMap>

#include "knmusictagbase.h"

class KNMusicTagID3v2 : public KNMusicTagBase
{
    Q_OBJECT
public:
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
        QMap<QString, QByteArray> frames;
    };
    static QString id3v2String(QByteArray value);

    explicit KNMusicTagID3v2(QObject *parent = 0);
    bool readTag(const QString &filePath);

signals:

public slots:

private:
    ID3v2Data m_tagData;
};

#endif // KNMUSICTAGID3V2_H
