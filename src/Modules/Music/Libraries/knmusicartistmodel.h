#ifndef KNMUSICARTISTMODEL_H
#define KNMUSICARTISTMODEL_H

#include "knmusicartistmodel.h"

#include <QIcon>

#include <QStandardItemModel>

class QSplitter;
class KNMusicModel;
class KNMusicArtistItem;
class KNMusicArtistModel : public QStandardItemModel
{
    Q_OBJECT
public:
    explicit KNMusicArtistModel(QObject *parent = 0);
    KNMusicArtistItem *artistItem(const QModelIndex &index);
    void resetModel();

signals:

public slots:
    void retranslate();
    void retranslateAndSet();
    void onMusicAdded(const QModelIndex &index);
    void setSourceModel(QAbstractItemModel *sourceModel);

private:
    KNMusicModel *m_sourceModel;
    QIcon m_noAlbumIcon;
    QString m_noArtist;
};

#endif // KNMUSICARTISTMODEL_H
