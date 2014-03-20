#ifndef KNMUSICTAGWMA_H
#define KNMUSICTAGWMA_H

#include "knmusictagbase.h"

class KNMusicTagWma : public KNMusicTagBase
{
    Q_OBJECT
public:
    explicit KNMusicTagWma(QObject *parent = 0);

    bool readTag(const QString &filePath);

signals:

public slots:

};

#endif // KNMUSICTAGWMA_H
