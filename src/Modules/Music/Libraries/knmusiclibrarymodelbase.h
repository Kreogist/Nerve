#ifndef KNMUSICLIBRARYMODELBASE_H
#define KNMUSICLIBRARYMODELBASE_H

#include "knmusicmodelbase.h"

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
    virtual void updateIndexInfo(const QModelIndex &index,
                                 const QString &filePath)=0;
    virtual void prepareRemove(const QModelIndex &removedIndex)=0;

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

};

#endif // KNMUSICLIBRARYMODELBASE_H
