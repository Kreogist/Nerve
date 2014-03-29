#include <QDebug>

#include "../knmusicglobal.h"

#include "knmusicartistitem.h"
#include "knmusiccategorymodel.h"

#include "knmusiccategorydetailmodel.h"

KNMusicCategoryDetailModel::KNMusicCategoryDetailModel(QObject *parent) :
    QSortFilterProxyModel(parent)
{
    m_nameFilter.setPattern("^$");
    m_nameFilter.setPatternSyntax(QRegExp::RegExp);
    setFilterRegExp(m_nameFilter);
}

void KNMusicCategoryDetailModel::setCategoryModel(KNMusicCategoryModel *artistModel)
{
    m_artistModel=artistModel;
}

void KNMusicCategoryDetailModel::setCategoryIndex(const QModelIndex &index)
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
