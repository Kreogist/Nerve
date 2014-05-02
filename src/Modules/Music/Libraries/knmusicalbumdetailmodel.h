#ifndef KNMUSICALBUMDETAILMODEL_H
#define KNMUSICALBUMDETAILMODEL_H

#include "knmusiccategorydetailmodel.h"

class KNMusicAlbumDetailModel : public KNMusicCategoryDetailModel
{
    Q_OBJECT
public:
    explicit KNMusicAlbumDetailModel(QObject *parent = 0);

signals:

public slots:
    void setCategoryIndex(const QModelIndex &index);

protected:
    QString getFilterText(const QModelIndex &index) const;

};

#endif // KNMUSICALBUMDETAILMODEL_H
