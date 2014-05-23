#ifndef KNMUSICINFOCOLLECTOR_H
#define KNMUSICINFOCOLLECTOR_H

#include <QStringList>
#include <QFile>
#include <QDataStream>

#include "../knmusicglobal.h"

#include "../../Base/knlibinfocollector.h"

class KNGlobal;
class KNMusicGlobal;
class KNLibMediaInfo;
class KNMusicTagID3v1;
class KNMusicTagID3v2;
class KNMusicTagAPEv2;
class KNMusicTagWMA;
class KNMusicTagM4A;
class KNMusicTagWAV;
class KNMusicTagFLAC;
class KNMusicInfoCollector : public KNLibInfoCollector
{
    Q_OBJECT
public:
    explicit KNMusicInfoCollector(QObject *parent = 0);

signals:
    void requireAppendMusic(QStringList value,
                            KNMusicGlobal::MusicDetailsInfo datas);
    void requireSkipCurrent();

public slots:
    void analysis(const QString &filePath);

private:
    void resetInfoCache();
    void parseByMediaInfo(const QString &value);
    void readID3v1Tag(QFile &mediaFile,
                      QDataStream &mediaData);
    void readID3v2Tag(QFile &mediaFile,
                      QDataStream &mediaData);
    void readAPEv2Tag(QFile &mediaFile,
                      QDataStream &mediaData);
    void readWMATag(QFile &mediaFile,
                    QDataStream &mediaData);
    void readM4ATag(QFile &mediaFile,
                    QDataStream &mediaData);
    void readFLACTag(QFile &mediaFile,
                     QDataStream &mediaData);
    void readWAVTag(QFile &mediaFile,
                    QDataStream &mediaData);
    void setMediaData(const int &index,
                      const QString &value);
    void setMusicCover(const QImage &pixmap);
    KNLibMediaInfo *m_mediaInfo;
    KNMusicTagID3v1 *m_tagID3v1;
    KNMusicTagID3v2 *m_tagID3v2;
    KNMusicTagAPEv2 *m_tagAPEv2;
    KNMusicTagWMA *m_tagWMA;
    KNMusicTagM4A *m_tagM4A;
    KNMusicTagFLAC *m_tagFLAC;
    KNMusicTagWAV *m_tagWAV;

    QString m_musicInfos[KNMusicGlobal::MusicDataCount];
    QImage m_musicCover;
    int m_musicRating, m_duration, m_bitRate, m_samplingRate;
    KNGlobal *m_global;
    KNMusicGlobal *m_musicGlobal;
};

#endif // KNMUSICINFOCOLLECTOR_H
