#ifndef KNMUSICTAGFLAC_H
#define KNMUSICTAGFLAC_H

#include <QMap>
#include "knmusictagbase.h"

class KNMusicTagFLAC : public KNMusicTagBase
{
    Q_OBJECT
public:
    explicit KNMusicTagFLAC(QObject *parent = 0);
    bool readTag(const QString &filePath);
    void clearCache();
    QString metaData(const QString &index);

signals:

public slots:

private:
    void parseVorbisComment(char *rawTagData, int length);
    quint32 flacCharToInt32(char *rawTagData);
    struct MetadataHeader
    {
        bool isLast=false;
        int type=127;
        quint32 size=0;
    };
    MetadataHeader analysisHeader(char *rawHeader);
    QMap<QString, QString> m_metadata;
};

#endif // KNMUSICTAGFLAC_H
