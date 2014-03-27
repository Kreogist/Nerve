#include <QDebug>

#include "../knmusicglobal.h"

#include "knmusicartistdetailmodel.h"
#include "knmusicartistitem.h"
#include "knmusicartistmodel.h"

KNMusicArtistDetailModel::KNMusicArtistDetailModel(QObject *parent) :
    QSortFilterProxyModel(parent)
{
    setFilterKeyColumn(KNMusicGlobal::Artist);
    m_nameFilter.setPattern("^$");
    m_nameFilter.setPatternSyntax(QRegExp::RegExp);
    setFilterRegExp(m_nameFilter);
}

void KNMusicArtistDetailModel::setArtistModel(KNMusicArtistModel *artistModel)
{
    m_artistModel=artistModel;
}

void KNMusicArtistDetailModel::setArtistIndex(const QModelIndex &index)
{
    if(index.row()==0)
    {
        m_nameFilter.setPattern("^$");
        m_nameFilter.setPatternSyntax(QRegExp::RegExp);
    }
    else
    {
        m_nameFilter.setPattern(m_artistModel->artistItem(index)->data(Qt::UserRole).toString());
        m_nameFilter.setPatternSyntax(QRegExp::FixedString);
    }
    setFilterRegExp(m_nameFilter);
    emit requireDetailSizeChange(rowCount());
}
