#ifndef KNMUSICCATEGORYSORTFILTERMODEL_H
#define KNMUSICCATEGORYSORTFILTERMODEL_H

#include <QSortFilterProxyModel>

class KNMusicCategorySortFilterModel : public QSortFilterProxyModel
{
    Q_OBJECT
public:
    explicit KNMusicCategorySortFilterModel(QObject *parent = 0);
    QVariant data(const QModelIndex &index, int role=Qt::DisplayRole) const;
    QModelIndex firstIndex() const;

signals:

public slots:

protected:
    bool lessThan(const QModelIndex &left, const QModelIndex &right) const;
    bool filterAcceptsRow(int source_row, const QModelIndex & source_parent) const;
};

#endif // KNMUSICCATEGORYSORTFILTERMODEL_H
