#ifndef KNMUSICTAGM4A_H
#define KNMUSICTAGM4A_H

#include <QMap>

#include "knmusictagbase.h"

class KNMusicTagM4A : public KNMusicTagBase
{
    Q_OBJECT
public:
    explicit KNMusicTagM4A(QObject *parent = 0);

    bool readTag(const QString &filePath);

signals:

public slots:

private:
    struct tagData
    {
        int start;
        int end;
    };

    void clearCache();
    void searchIn(const char *rawTagData,
                  const int &start,
                  const int &end);
    char m_m4aHeader[4]={'f', 't', 'y', 'p'};
    char m_idCache[5];
    QByteArray m_rawData;
    QMap<QString, tagData> m_tagData;
};

#endif // KNMUSICTAGM4A_H
