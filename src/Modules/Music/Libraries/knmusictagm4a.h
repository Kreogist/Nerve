#ifndef KNMUSICTAGM4A_H
#define KNMUSICTAGM4A_H

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
    char m_m4aHeader[4]={'f', 't', 'y', 'p'};
};

#endif // KNMUSICTAGM4A_H
