#ifndef KNMUSICINFOCOLLECTOR_H
#define KNMUSICINFOCOLLECTOR_H

#include <QObject>
#include <QStringList>

#include "../knmusicglobal.h"

class KNMusicTagID3v1;
class KNMusicTagID3v2;
class KNMusicInfoCollector : public QObject
{
    Q_OBJECT
public:
    explicit KNMusicInfoCollector(QObject *parent = 0);

signals:
    void requireAppendMusic(const QStringList &value);

public slots:
    void analysis(const QString &filePath);

private:
    void resetMusicInfo();
    void readID3v1Tag(const QString &value);
    void readID3v2Tag(const QString &value);
    KNMusicTagID3v1 *m_tagID3v1;
    KNMusicTagID3v2 *m_tagID3v2;

    QString m_musicInfos[KNMusicGlobal::MusicDataCount];
};

#endif // KNMUSICINFOCOLLECTOR_H
