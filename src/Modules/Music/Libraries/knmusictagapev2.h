#ifndef KNMUSICTAGAPEV2_H
#define KNMUSICTAGAPEV2_H

#include <QDataStream>

#include "knmusictagbase.h"

class KNMusicTagAPEv2 : public KNMusicTagBase
{
    Q_OBJECT
public:
    explicit KNMusicTagAPEv2(QObject *parent = 0);

    bool readTag(const QString &filePath);

signals:

public slots:

private:
    void clearCache();
    bool checkAPEHeaderAt(int position, QDataStream &mediaData);
    char m_apePreamble[9]={'A', 'P', 'E', 'T', 'A', 'G', 'E', 'X', '\0'};

    char m_apeHeader[32];
    char m_preambleCheck[9];
};

#endif // KNMUSICTAGAPEV2_H
