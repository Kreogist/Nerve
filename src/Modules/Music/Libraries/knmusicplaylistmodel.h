#ifndef KNMUSICPLAYLISTMODEL_H
#define KNMUSICPLAYLISTMODEL_H

#include "knmusicmodelbase.h"

class KNMusicPlaylistModel : public KNMusicModelBase
{
    Q_OBJECT
public:
    explicit KNMusicPlaylistModel(QObject *parent = 0);
    void resetHeader();

signals:

public slots:

};

#endif // KNMUSICPLAYLISTMODEL_H
