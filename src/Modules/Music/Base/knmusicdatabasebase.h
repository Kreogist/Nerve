#ifndef KNMUSICDATABASEBASE_H
#define KNMUSICDATABASEBASE_H

#include "../../Base/knstdlibdatabase.h"

class KNMusicDatabaseBase : public KNStdLibDatabase
{
    Q_OBJECT
public:
    explicit KNMusicDatabaseBase(QObject *parent = 0);

signals:

public slots:

protected slots:
    virtual void onActionRowAppend(QModelIndex index)=0;
    virtual void onActionRowUpdate(QModelIndex index)=0;
    virtual void onActionRowRemove(QModelIndex index)=0;
    virtual void onActionUpdateCoverImage(const int &index)=0;

};

#endif // KNMUSICDATABASEBASE_H
