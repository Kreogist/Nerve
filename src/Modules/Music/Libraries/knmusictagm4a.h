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

};

#endif // KNMUSICTAGM4A_H
