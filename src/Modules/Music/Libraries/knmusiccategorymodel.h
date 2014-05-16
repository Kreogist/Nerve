#ifndef KNMUSICCATEGORYMODEL_H
#define KNMUSICCATEGORYMODEL_H

#include <QIcon>

#include "knmusicmodel.h"

#include <QStandardItemModel>

class QSplitter;
class KNMusicArtistItem;
class KNMusicCategoryModel : public QStandardItemModel
{
    Q_OBJECT
public:
    explicit KNMusicCategoryModel(QObject *parent = 0);
    KNMusicArtistItem *artistItem(const QModelIndex &index);
    void resetModel();
    QString noCategoryText() const;
    void setNoCategoryText(const QString &noCategoryText);

signals:
    void requireHideFirstItem();
    void requireShowFirstItem();

public slots:
    virtual void retranslate();
    virtual void retranslateAndSet();
    virtual void onMusicAdded(const QModelIndex &index);
    virtual void onMusicRecover(const QModelIndex &index);
    virtual void onMusicRemoved(const QModelIndex &index);
    virtual void onAlbumArtUpdate(const int &index);
    void setSourceModel(QAbstractItemModel *sourceModel);


protected:
    virtual void updateImage(const int &index);
    virtual QIcon itemIcon(const int &index) const;
    virtual QString categoryName(const int &index) const;
    KNMusicModel *m_sourceModel;
    int m_noCategoryItemCount=0;

private slots:
    void updateAllImage();

private:
    QIcon m_noAlbumArtIcon;
    QString m_noCategoryText;
};

#endif // KNMUSICCATEGORYMODEL_H
