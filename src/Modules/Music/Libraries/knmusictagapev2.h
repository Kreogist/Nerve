#ifndef KNMUSICTAGAPEV2_H
#define KNMUSICTAGAPEV2_H

#include <QDataStream>
#include <QMap>

#include "knmusictagbase.h"

class KNMusicTagAPEv2 : public KNMusicTagBase
{
    Q_OBJECT
public:
    enum APEv2Items
    {
        Name,
        Artist,
        Album,
        Comments,
        Composer,
        Genre,
        Year,
        Track,
        APEv2ItemsCount
    };

    explicit KNMusicTagAPEv2(QObject *parent = 0);
    QString textData(const int &key) const;
    bool readTag(const QFile &mediaFile,
                 QDataStream &mediaData);
    void clearCache();

signals:

public slots:

private:
    bool checkAPEHeaderAt(QDataStream &mediaData);
    bool readTagAt(QDataStream &mediaData);
    const char m_apePreamble[9]={'A', 'P', 'E', 'T', 'A', 'G', 'E', 'X', '\0'};

    char m_apeHeader[32];
    char m_preambleCheck[9];
    int m_headerPosition;
    quint32 m_versionNumber;
    quint32 m_tagSize;
    quint32 m_itemCount;
    quint32 m_tagsFlags;
    QMap<QString, QByteArray> m_frameDatas;
    QString m_frames[APEv2ItemsCount];
};

#endif // KNMUSICTAGAPEV2_H
