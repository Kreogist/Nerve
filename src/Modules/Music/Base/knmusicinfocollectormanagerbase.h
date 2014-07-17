#ifndef KNMUSICINFOCOLLECTORMANAGERBASE_H
#define KNMUSICINFOCOLLECTORMANAGERBASE_H

#include <QStringList>

#include "../knmusicglobal.h"

#include "../../Base/knlibinfocollectormanager.h"

class QStandardItem;
class KNMusicBackend;
class KNMusicInfoCollectorManagerBase : public KNLibInfoCollectorManager
{
    Q_OBJECT
public:
    explicit KNMusicInfoCollectorManagerBase(QObject *parent = 0);
    virtual QStringList currentFileData() const=0;
    virtual KNMusicGlobal::MusicDetailsInfo currentFileAppendData() const=0;
    virtual QStandardItem *currentIndex() const=0;
    virtual void removeFirstUpdateResult()=0;
    virtual bool isUpdateQueueEmpty()=0;
    virtual bool isWorking()=0;
    virtual void setMusicBackend(KNMusicBackend *backend)=0;

signals:
    void requireAnalysis(const QString &filePath);

public slots:

};

#endif // KNMUSICINFOCOLLECTORMANAGERBASE_H
