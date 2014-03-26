#ifndef KNMUSICARTISTDETAILMODEL_H
#define KNMUSICARTISTDETAILMODEL_H

#include <QSortFilterProxyModel>

class KNMusicArtistModel;
class KNMusicArtistDetailModel : public QSortFilterProxyModel
{
    Q_OBJECT
public:
    explicit KNMusicArtistDetailModel(QObject *parent = 0);
    void setArtistModel(KNMusicArtistModel *artistModel);
    void reset();

signals:

public slots:
    void setArtistIndex(const QModelIndex &index);

private:
    KNMusicArtistModel *m_artistModel;
};

#endif // KNMUSICARTISTDETAILMODEL_H
