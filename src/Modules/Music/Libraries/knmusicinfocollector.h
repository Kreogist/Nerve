#ifndef KNMUSICINFOCOLLECTOR_H
#define KNMUSICINFOCOLLECTOR_H

#include <QObject>
#include <QStringList>
#include <QPixmap>

#include "../knmusicglobal.h"

class KNGlobal;
class KNMusicGlobal;
class KNMusicTagID3v1;
class KNMusicTagID3v2;
class KNMusicTagAPEv2;
class KNMusicTagWma;
class KNMusicInfoCollector : public QObject
{
    Q_OBJECT
public:
    explicit KNMusicInfoCollector(QObject *parent = 0);

signals:
    void requireAppendMusic(const QStringList &value,
                            const QPixmap &coverImage);

public slots:
    void analysis(const QString &filePath);

private:
    void resetMusicInfo();
    void readID3v1Tag(const QString &value);
    void readID3v2Tag(const QString &value);
    void readAPEv2Tag(const QString &value);
    void readWMATag(const QString &value);
    void setMediaData(const int &index,
                      const QString &value);
    KNMusicTagID3v1 *m_tagID3v1;
    KNMusicTagID3v2 *m_tagID3v2;
    KNMusicTagAPEv2 *m_tagAPEv2;
    KNMusicTagWma *m_tagWMA;

    QString m_musicInfos[KNMusicGlobal::MusicDataCount];
    QPixmap m_musicCover;
    KNGlobal *m_global;
    KNMusicGlobal *m_musicGlobal;
};

#endif // KNMUSICINFOCOLLECTOR_H
