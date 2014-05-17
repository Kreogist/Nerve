#ifndef KNMUSICTAGBASE_H
#define KNMUSICTAGBASE_H

#include <QFile>
#include <QDataStream>

#include <QObject>

class KNMusicTagBase : public QObject
{
    Q_OBJECT
public:
    explicit KNMusicTagBase(QObject *parent = 0);
    virtual void clearCache();
    virtual bool readTag(const QFile &mediaFile,
                         QDataStream &mediaData);
    virtual QString textData(const int &key) const;

signals:

public slots:

};

#endif // KNMUSICTAGBASE_H
