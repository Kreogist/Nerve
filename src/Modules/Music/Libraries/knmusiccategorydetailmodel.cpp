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

void KNMusicCategoryDetailModel::setCategoryModel(KNMusicCategoryModel *categoryModel)
{
    m_artistModel=categoryModel;
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
        m_nameFilter.setPattern(getFilterText(index));
        m_nameFilter.setPatternSyntax(QRegExp::FixedString);
    }
    setFilterRegExp(m_nameFilter);
    emit requireSongCountChange(rowCount());
}

QString KNMusicCategoryDetailModel::getFilterText(const QModelIndex &index) const
{
    return m_artistModel->filterString(index);
}

bool KNMusicCategoryDetailModel::lessThan(const QModelIndex &left,
                                          const QModelIndex &right) const
{
    QVariant leftData=sourceModel()->data(left);
    QVariant rightData=sourceModel()->data(right);
    QVariant sortFlag=sourceModel()->headerData(left.column(),
                                                Qt::Horizontal,
                                                Qt::UserRole);
    if(!sortFlag.isValid())
    {
        return QString::compare(leftData.toString(),
                                rightData.toString())<0;
    }
    int sortIndex=sortFlag.toInt();
    QVariant leftUserData=sourceModel()->data(left, Qt::UserRole);
    QVariant rightUserData=sourceModel()->data(right, Qt::UserRole);
    switch(sortIndex)
    {
    case 1:
        return leftData.toInt()<rightData.toInt();
    case 2:
        return leftUserData.toInt()<rightUserData.toInt();
    }
    return QString::compare(leftData.toString(),
                            rightData.toString())<0;
}
