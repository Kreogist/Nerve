#ifndef KNMUSICARTISTMODEL_H
#define KNMUSICARTISTMODEL_H

#include "knmusicartistmodel.h"

#include <QStandardItemModel>

class KNMusicModel;
class KNMusicArtistModel : public QStandardItemModel
{
    Q_OBJECT
public:
    explicit KNMusicArtistModel(QObject *parent = 0);

signals:

public slots:
    void onMusicAdded(const QModelIndex &index);
    void setSourceModel(QAbstractItemModel *sourceModel);

private:
    KNMusicModel *m_sourceModel;
};

#endif // KNMUSICARTISTMODEL_H
