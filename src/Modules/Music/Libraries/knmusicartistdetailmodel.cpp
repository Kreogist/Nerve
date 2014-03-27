#include <QDebug>

#include "../knmusicglobal.h"

#include "knmusicartistdetailmodel.h"
#include "knmusicartistitem.h"
#include "knmusicartistmodel.h"

KNMusicArtistDetailModel::KNMusicArtistDetailModel(QObject *parent) :
    QSortFilterProxyModel(parent)
{
    setFilterKeyColumn(KNMusicGlobal::Artist);
    m_nameFilter.setPattern("");
    m_nameFilter.setPatternSyntax(QRegExp::FixedString);
    setFilterRegExp(m_nameFilter);
}

void KNMusicArtistDetailModel::setArtistModel(KNMusicArtistModel *artistModel)
{
    m_artistModel=artistModel;
}

void KNMusicArtistDetailModel::setArtistIndex(const QModelIndex &index)
{
    m_nameFilter.setPattern(m_artistModel->artistItem(index)->text());
    setFilterRegExp(m_nameFilter);
    emit requireDetailSizeChange(rowCount());
}
