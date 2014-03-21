#include "knmusicsortmodel.h"

KNMusicSortModel::KNMusicSortModel(QObject *parent) :
    QSortFilterProxyModel(parent)
{
}

bool KNMusicSortModel::lessThan(const QModelIndex &left,
                                const QModelIndex &right) const
{
    /*QVariant leftData=sourceModel()->data(left);
    QVariant rightData=sourceModel()->data(right);*/
    //sourceModel()->headerData(left.column(), Qt::Horizontal);
    return QSortFilterProxyModel::lessThan(left, right);
}
