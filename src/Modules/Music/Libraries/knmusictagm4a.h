#ifndef KNMUSICTAGM4A_H
#define KNMUSICTAGM4A_H

#include <QList>

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
    struct m4aAtom
    {
        QString id;
        int start;
        int end;
    };

    void readAtom(m4aAtom &atomData);
    char m_m4aHeader[4]={'f', 't', 'y', 'p'};
    char m_idCache[5];
    QList<m4aAtom> m_atomList;
};

#endif // KNMUSICTAGM4A_H
