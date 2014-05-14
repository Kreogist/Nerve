#ifndef KNMUSICDATABASE_H
#define KNMUSICDATABASE_H

#include "../../Base/knlibmodeldatabase.h"

class KNMusicModel;
class KNMusicDatabase : public KNLibModelDatabase
{
    Q_OBJECT
public:
    explicit KNMusicDatabase(QObject *parent = 0);
    void setModel(KNMusicModel *model);

signals:

public slots:

private:
    KNMusicModel *m_model;
};

#endif // KNMUSICDATABASE_H
