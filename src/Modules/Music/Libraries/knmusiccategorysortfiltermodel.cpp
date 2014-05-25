#include <QSize>

#include <QDebug>

#include "knmusiccategorysortfiltermodel.h"

KNMusicCategorySortFilterModel::KNMusicCategorySortFilterModel(QObject *parent) :
    QSortFilterProxyModel(parent)
{
}

QVariant KNMusicCategorySortFilterModel::data(const QModelIndex &index, int role) const
{
    if(role==Qt::SizeHintRole)
    {
        return QSize(35, 40);
    }
    return QSortFilterProxyModel::data(index, role);
}

QModelIndex KNMusicCategorySortFilterModel::firstIndex() const
{
    if(sourceModel()->data(sourceModel()->index(0,0),
                           Qt::UserRole+1).toInt()==0)
    {
        return index(1,0);
    }
    return index(0,0);
}

bool KNMusicCategorySortFilterModel::lessThan(const QModelIndex &left,
                                              const QModelIndex &right) const
{
    if(left.row()==0)
    {
        return true;
    }
    if(right.row()==0)
    {
        return false;
    }
    return QSortFilterProxyModel::lessThan(left, right);
}

bool KNMusicCategorySortFilterModel::filterAcceptsRow(int source_row,
                                                      const QModelIndex &source_parent) const
{
    if(source_row==0 && sourceModel()->data(sourceModel()->index(0,0),
                                            Qt::UserRole+1).toInt()==0)
    {
        return false;
    }
    return QSortFilterProxyModel::filterAcceptsRow(source_row, source_parent);
}
