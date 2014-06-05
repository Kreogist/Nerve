#ifndef KNMUSICTAGID3V1_H
#define KNMUSICTAGID3V1_H

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
    void setTextData(const int &key, const QString &data);
    void writeTag(QFile &mediaFile,
                  QDataStream &mediaData);
    quint8 genreIndex() const;

signals:

public slots:

private:
    QString m_tagData[ID3v1ItemCount];
    char m_rawTagData[128], m_rawByteBackup, m_emptyTag[128];
    QTextCodec *m_localCodec;
    quint8 m_genreIndex;
};

#endif // KNMUSICTAGID3V1_H
