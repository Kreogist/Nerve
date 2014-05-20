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
