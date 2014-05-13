#include <QDateTime>

#include <QDebug>

#include "knmusicsortmodel.h"

KNMusicSortModel::KNMusicSortModel(QObject *parent) :
    QSortFilterProxyModel(parent)
{
    ;
}

void KNMusicSortModel::removeOriginalItem(const QModelIndex &index)
{
    sourceModel()->removeRow(index.row());
}

bool KNMusicSortModel::lessThan(const QModelIndex &left,
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
    case 3:
        return leftUserData.toFloat()<rightUserData.toFloat();
    case 4:
        return leftUserData.toDateTime()<rightUserData.toDateTime();
    }
    return QString::compare(leftData.toString(),
                            rightData.toString())<0;
}
