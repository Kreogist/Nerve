#include <QDebug>

#include "knmusiccategorysortfiltermodel.h"

KNMusicCategorySortFilterModel::KNMusicCategorySortFilterModel(QObject *parent) :
    QSortFilterProxyModel(parent)
{
}

void KNMusicCategorySortFilterModel::setFilterFixedString(const QString &pattern)
{
    QSortFilterProxyModel::setFilterFixedString(pattern);
}
