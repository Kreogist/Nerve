#ifndef KNMUSICLIBRARYMODELBASE_H
#define KNMUSICLIBRARYMODELBASE_H

#include "knmusicmodelbase.h"

class KNMusicLibraryModelBase : public KNMusicModelBase
{
    Q_OBJECT
public:
    explicit KNMusicLibraryModelBase(QObject *parent = 0);

signals:
    void musicAppend(QModelIndex index);
    void musicAboutToRemove(QModelIndex index);
    void musicUpdate(QModelIndex index);
    void musicRecover(QModelIndex index);
    void musicAlbumArtUpdate(int index);
    void requireLoadImage();
    void requireUpdateImage();

public slots:

};

#endif // KNMUSICLIBRARYMODELBASE_H
