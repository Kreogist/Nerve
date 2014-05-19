#ifndef KNMUSICCATEGORYSORTFILTERMODEL_H
#define KNMUSICCATEGORYSORTFILTERMODEL_H

#include <QSortFilterProxyModel>

class KNMusicCategorySortFilterModel : public QSortFilterProxyModel
{
    Q_OBJECT
public:
    explicit KNMusicCategorySortFilterModel(QObject *parent = 0);
    bool isNoCategoryShown();

signals:

public slots:

protected:
    bool lessThan(const QModelIndex &left, const QModelIndex &right) const;
};

#endif // KNMUSICCATEGORYSORTFILTERMODEL_H
