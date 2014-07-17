#ifndef KNMUSICLIBRARYMODELBASE_H
#define KNMUSICLIBRARYMODELBASE_H

#include <QStringList>

#include "knmusicmodelbase.h"

class KNLibInfoCollectorManager;
class KNMusicLibraryModelBase : public KNMusicModelBase
{
    Q_OBJECT
public:
    explicit KNMusicLibraryModelBase(QObject *parent = 0);
    virtual QImage artwork(const int &row) const=0;
    virtual QImage artworkFromKey(const QString &key) const=0;
    virtual QString artworkKey(const int &row) const=0;
    virtual void setAlbumArtPath(const QString &path)=0;
    virtual void recoverFile(QStringList textList,
                             KNMusicGlobal::MusicDetailsInfo currentDetails)=0;
    virtual void updateIndexInfo(QStandardItem *index,
                                 const QString &filePath)=0;
    virtual void removeAppendData(const QModelIndex &removedIndex)=0;
    KNLibInfoCollectorManager *infoCollectorManager();
    virtual void setInfoCollectorManager(KNLibInfoCollectorManager *infoCollectorManager);

signals:
    void musicAppend(QModelIndex index);
    void musicAboutToRemove(QModelIndex index);
    void musicUpdate(QModelIndex index);
    void musicRecover(QModelIndex index);
    void musicAlbumArtUpdate(int index);
    void requireLoadImage();
    void requireUpdateImage();

public slots:
    virtual void onActionRecoverComplete()=0;

private:
    KNLibInfoCollectorManager *m_infoCollectorManager;
};

#endif // KNMUSICLIBRARYMODELBASE_H
