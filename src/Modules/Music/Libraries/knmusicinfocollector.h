#ifndef KNMUSICINFOCOLLECTOR_H
#define KNMUSICINFOCOLLECTOR_H

#include <QStringList>

#include "../knmusicglobal.h"

#include "../../Base/knlibinfocollector.h"

class KNGlobal;
class KNMusicGlobal;
class KNLibMediaInfo;
class KNMusicTagID3v1;
class KNMusicTagID3v2;
class KNMusicTagAPEv2;
class KNMusicTagWma;
class KNMusicTagM4A;
class KNMusicTagFLAC;
class KNMusicInfoCollector : public KNLibInfoCollector
{
    Q_OBJECT
public:
    explicit KNMusicInfoCollector(QObject *parent = 0);

signals:
    void requireAppendMusic(const QStringList &value,
                            const KNMusicGlobal::MusicDetailsInfo &datas);
    void requireSkipCurrent();

public slots:
    void analysis(const QString &filePath);

private:
    void resetInfoCache();
    void parseByMediaInfo(const QString &value);
    void readID3v1Tag(const QString &value);
    void readID3v2Tag(const QString &value);
    void readAPEv2Tag(const QString &value);
    void readWMATag(const QString &value);
    void readM4ATag(const QString &value);
    void readFLACTag(const QString &value);
    void setMediaData(const int &index,
                      const QString &value);
    KNLibMediaInfo *m_mediaInfo;
    KNMusicTagID3v1 *m_tagID3v1;
    KNMusicTagID3v2 *m_tagID3v2;
    KNMusicTagAPEv2 *m_tagAPEv2;
    KNMusicTagWma *m_tagWMA;
    KNMusicTagM4A *m_tagM4A;
    KNMusicTagFLAC *m_tagFLAC;

    QString m_musicInfos[KNMusicGlobal::MusicDataCount];
    QPixmap m_musicCover;
    int m_musicRating, m_duration, m_bitRate, m_samplingRate;
    KNGlobal *m_global;
    KNMusicGlobal *m_musicGlobal;
};

#endif // KNMUSICINFOCOLLECTOR_H
