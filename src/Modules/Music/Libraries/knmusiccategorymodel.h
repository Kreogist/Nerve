#ifndef KNMUSICCATEGORYMODEL_H
#define KNMUSICCATEGORYMODEL_H

#include <QIcon>
#include <QList>

#include "knmusicmodel.h"

#include <QAbstractListModel>

class KNMusicCategoryModel : public QAbstractListModel
{
    Q_OBJECT
public:
    explicit KNMusicCategoryModel(QObject *parent = 0);
    int rowCount(const QModelIndex &parent = QModelIndex()) const;
    int columnCount(const QModelIndex &parent = QModelIndex()) const;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;
    QVariant headerData(int section, Qt::Orientation orientation, int role) const;
    bool setData(const QModelIndex &index, const QVariant &value, int role);
    QModelIndex indexOf(const QString &text) const;
    bool removeRows(int row, int count, const QModelIndex &parent=QModelIndex());
    virtual void resetModel();
    virtual QString filterText(const QModelIndex &index) const;
    QString noCategoryText() const;
    void setNoCategoryText(const QString &noCategoryText);
    QString filterString(const QModelIndex &index) const;

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
    struct MusicCategoryItem
    {
        QIcon decoration;
        QString iconKey;
        int songCount;
    };
    MusicCategoryItem item(const int &index) const;
    virtual void updateImage(const int &index);
    virtual QIcon itemIcon(const int &index) const;
    virtual QString categoryName(const int &index) const;
    virtual void onActionRemoveRow(const int &index);
    KNMusicModel *m_sourceModel;
    int m_noCategoryItemCount=0;
    QIcon m_noAlbumArtIcon;
    QStringList m_textList;
    QList<MusicCategoryItem> m_detailList;

private slots:
    void updateAllImage();

private:
    QString m_noCategoryText;
};

#endif // KNMUSICCATEGORYMODEL_H
