#ifndef KNMUSICCATEGORYDETAILMODEL_H
#define KNMUSICCATEGORYDETAILMODEL_H

#include <QRegExp>

#include <QSortFilterProxyModel>

class KNMusicCategoryModel;
class KNMusicCategoryDetailModel : public QSortFilterProxyModel
{
    Q_OBJECT
public:
    explicit KNMusicCategoryDetailModel(QObject *parent = 0);
    void setCategoryModel(KNMusicCategoryModel *artistModel);
    void reset();

signals:
    void requireSongCountChange(const int &value);

public slots:
    void setCategoryIndex(const QModelIndex &index);

protected:
    virtual QString getFilterText(const QModelIndex &index) const;
    bool lessThan(const QModelIndex &left, const QModelIndex &right) const;
    KNMusicCategoryModel *m_artistModel;

private:
    QRegExp m_nameFilter;
};

#endif // KNMUSICCATEGORYDETAILMODEL_H
