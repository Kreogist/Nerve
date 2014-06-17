#ifndef KNMUSICTAGWAV_H
#define KNMUSICTAGWAV_H

#include "knmusictagbase.h"

class KNMusicTagID3v2;
class KNMusicTagWAV : public KNMusicTagBase
{
    Q_OBJECT
public:
    enum WAVItems
    {
        Name,
        Artist,
        Album,
        AlbumArtist,
        BeatsPerMinuate,
        Category,
        Comments,
        Composer,
        Description,
        Genre,
        Track,
        Disc,
        Rating,
        Year,
        WAVItemCount
    };
    explicit KNMusicTagWAV(QObject *parent = 0);
    void clearCache();
    bool readTag(const QFile &mediaFile,
                 QDataStream &mediaData);
    QString textData(const int &key) const;

signals:

public slots:

private:
    void parseLIST();
    void parseID32();
    void setTagData(const int &index, const QString &data);
    char m_wavHeader[12], m_wavChunkHeader[8], *m_rawChunkData;
    quint32 m_chunkSize;
    bool m_findList, m_findID3;
    QString m_wavFrameData[WAVItemCount];
    KNMusicTagID3v2 *m_id3v2Parser;
};

#endif // KNMUSICTAGWAV_H
