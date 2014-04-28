#ifndef KNMUSICINFOCOLLECTOR_H
#define KNMUSICINFOCOLLECTOR_H

#include <QStringList>

#include "../knmusicglobal.h"

#include "../../Base/knlibinfocollector.h"

class KNGlobal;
class KNMusicGlobal;
class KNMusicTagID3v1;
class KNMusicTagID3v2;
class KNMusicTagAPEv2;
class KNMusicTagWma;
class KNMusicTagM4A;
class KNMusicInfoCollector : public KNLibInfoCollector
{
    Q_OBJECT
public:
    explicit KNMusicInfoCollector(QObject *parent = 0);

signals:
    void requireAppendMusic(const QStringList &value,
                            const KNMusicGlobal::MusicDetailsInfo &info);
    void requireSkipCurrent();

public slots:
    void analysis(const QString &filePath);

private:
    void resetMusicInfo();
    void readID3v1Tag(const QString &value);
    void readID3v2Tag(const QString &value);
    void readAPEv2Tag(const QString &value);
    void readWMATag(const QString &value);
    void readM4ATag(const QString &value);
    void setMediaData(const int &index,
                      const QString &value);
    KNMusicTagID3v1 *m_tagID3v1;
    KNMusicTagID3v2 *m_tagID3v2;
    KNMusicTagAPEv2 *m_tagAPEv2;
    KNMusicTagWma *m_tagWMA;
    KNMusicTagM4A *m_tagM4A;

    QString m_musicInfos[KNMusicGlobal::MusicDataCount];
    QPixmap m_musicCover;
    int m_musicRating;
    KNGlobal *m_global;
    KNMusicGlobal *m_musicGlobal;
};

#endif // KNMUSICINFOCOLLECTOR_H
