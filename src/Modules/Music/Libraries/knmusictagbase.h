#ifndef KNMUSICTAGBASE_H
#define KNMUSICTAGBASE_H

#include <QObject>

class KNMusicTagBase : public QObject
{
    Q_OBJECT
public:
    explicit KNMusicTagBase(QObject *parent = 0);
    virtual void clearCache();
    virtual bool readTag(const QString &filePath);

signals:

public slots:

};

#endif // KNMUSICTAGBASE_H
