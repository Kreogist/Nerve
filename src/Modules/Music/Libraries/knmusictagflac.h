#ifndef KNMUSICTAGFLAC_H
#define KNMUSICTAGFLAC_H

#include <QMap>
#include <QPixmap>
#include "knmusictagbase.h"

class KNMusicTagFLAC : public KNMusicTagBase
{
    Q_OBJECT
public:
    explicit KNMusicTagFLAC(QObject *parent = 0);
    bool readTag(const QString &filePath);
    void clearCache();
    QString metaData(const QString &index);
    QString rawMetaData(const QString &index);
    QPixmap tagImage(const int &index) const;
    QPixmap firstAvaliableImage() const;

signals:

public slots:

private:
    void parseVorbisComment(char *rawTagData, int length);
    void parsePicture(char *rawTagData);
    quint32 inverseCharToInt32(char *rawTagData);
    quint32 charToInt32(char *rawTagData);
    struct MetadataHeader
    {
        bool isLast=false;
        int type=127;
        quint32 size=0;
    };
    MetadataHeader analysisHeader(char *rawHeader);
    QMap<QString, QString> m_metadata;
    QMap<int, QPixmap> m_picture;
};

#endif // KNMUSICTAGFLAC_H