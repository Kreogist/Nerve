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
    void requireDetailSizeChange(const int &value);

public slots:
    void setCategoryIndex(const QModelIndex &index);

private:
    KNMusicCategoryModel *m_artistModel;
    QRegExp m_nameFilter;
};

#endif // KNMUSICCATEGORYDETAILMODEL_H
