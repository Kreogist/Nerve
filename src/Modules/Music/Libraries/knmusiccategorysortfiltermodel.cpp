#include <QDebug>

#include "knmusiccategorysortfiltermodel.h"

KNMusicCategorySortFilterModel::KNMusicCategorySortFilterModel(QObject *parent) :
    QSortFilterProxyModel(parent)
{
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
    return QString::compare(sourceModel()->data(left,  filterRole()).toString(),
                            sourceModel()->data(right, filterRole()).toString())<0;
}
