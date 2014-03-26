#include <QDebug>

#include "../knmusicglobal.h"

#include "knmusicartistdetailmodel.h"
#include "knmusicartistitem.h"
#include "knmusicartistmodel.h"

KNMusicArtistDetailModel::KNMusicArtistDetailModel(QObject *parent) :
    QSortFilterProxyModel(parent)
{
    setFilterKeyColumn(KNMusicGlobal::Artist);
    setFilterFixedString(" ");
}

void KNMusicArtistDetailModel::setArtistModel(KNMusicArtistModel *artistModel)
{
    m_artistModel=artistModel;
}

void KNMusicArtistDetailModel::setArtistIndex(const QModelIndex &index)
{
    setFilterFixedString(m_artistModel->artistItem(index)->text());
}
