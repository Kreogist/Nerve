#ifndef KNMUSICCATEGORYSORTFILTERMODEL_H
#define KNMUSICCATEGORYSORTFILTERMODEL_H

#include <QSortFilterProxyModel>

class KNMusicCategorySortFilterModel : public QSortFilterProxyModel
{
    Q_OBJECT
public:
    explicit KNMusicCategorySortFilterModel(QObject *parent = 0);
    void setFilterFixedString(const QString & pattern);

signals:

public slots:

};

#endif // KNMUSICCATEGORYSORTFILTERMODEL_H
