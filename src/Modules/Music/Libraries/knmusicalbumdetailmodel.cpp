#include "knmusicartistitem.h"
#include "knmusiccategorymodel.h"

#include "../knmusicglobal.h"

#include "knmusicalbumdetailmodel.h"

KNMusicAlbumDetailModel::KNMusicAlbumDetailModel(QObject *parent) :
    KNMusicCategoryDetailModel(parent)
{
    setSortRole(Qt::UserRole);
}

void KNMusicAlbumDetailModel::setCategoryIndex(const QModelIndex &index)
{
    KNMusicCategoryDetailModel::setCategoryIndex(index);
    sort(KNMusicGlobal::TrackNumber);
}

QString KNMusicAlbumDetailModel::getFilterText(const QModelIndex &index) const
{
    return m_artistModel->artistItem(index)->data(Qt::DisplayRole).toString();
}
