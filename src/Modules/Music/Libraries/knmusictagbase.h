#ifndef KNMUSICTAGBASE_H
#define KNMUSICTAGBASE_H

#include <QObject>

class KNMusicTagBase : public QObject
{
    Q_OBJECT
public:
    explicit KNMusicTagBase(QObject *parent = 0);
    virtual void clearCache();

signals:

public slots:

};

#endif // KNMUSICTAGBASE_H
