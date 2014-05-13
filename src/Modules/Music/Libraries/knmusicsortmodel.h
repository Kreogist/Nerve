#ifndef KNMUSICSORTMODEL_H
#define KNMUSICSORTMODEL_H

#include <QSortFilterProxyModel>

class KNMusicSortModel : public QSortFilterProxyModel
{
    Q_OBJECT
public:
    explicit KNMusicSortModel(QObject *parent = 0);
    void removeOriginalItem(const QModelIndex &index);

signals:

public slots:

protected:
    bool lessThan(const QModelIndex &left, const QModelIndex &right) const;
};

#endif // KNMUSICSORTMODEL_H
