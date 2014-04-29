#include "knmusicartistitem.h"
#include "knmusiccategorymodel.h"

#include "knmusicalbumdetailmodel.h"

KNMusicAlbumDetailModel::KNMusicAlbumDetailModel(QObject *parent) :
    KNMusicCategoryDetailModel(parent)
{
}

QString KNMusicAlbumDetailModel::getFilterText(const QModelIndex &index) const
{
    return m_artistModel->artistItem(index)->data(Qt::DisplayRole).toString();
}
