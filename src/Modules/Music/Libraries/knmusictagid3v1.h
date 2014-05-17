#ifndef KNMUSICTAGID3V1_H
#define KNMUSICTAGID3V1_H

#include <QFile>
#include <QDataStream>

#include <QStringList>

#include "knmusictagbase.h"

class QTextCodec;
class KNMusicTagID3v1 : public KNMusicTagBase
{
    Q_OBJECT
public:
    enum ID3v1Items
    {
        Title,
        Artist,
        Album,
        Year,
        Comment,
        Track,
        Genre,
        ID3v1ItemCount
    };
    explicit KNMusicTagID3v1(QObject *parent = 0);
    bool readTag(const QFile &mediaFile,
                 QDataStream &mediaData);
    void clearCache();
    QString textData(const int &key) const;

signals:

public slots:

private:
    QString getGenre(const int &index);
    QString m_tagData[ID3v1ItemCount];
    QTextCodec *m_codec;
};

#endif // KNMUSICTAGID3V1_H
