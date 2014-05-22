#ifndef KNMUSICCATEGORYMODEL_H
#define KNMUSICCATEGORYMODEL_H

#include <QIcon>
#include <QList>

#include "knmusicmodel.h"

#include <QStandardItemModel>

class KNMusicCategoryModel : public QStandardItemModel
{
    Q_OBJECT
public:
    explicit KNMusicCategoryModel(QObject *parent = 0);
    QModelIndex indexOf(const QString &text) const;
    QModelIndex firstItemIndex() const;
    virtual void resetModel();
    virtual QString filterText(const QModelIndex &index) const;
    QString noCategoryText() const;
    void setNoCategoryText(const QString &noCategoryText);
    QString filterString(const QModelIndex &index) const;
    QIcon albumArt(const QModelIndex &index) const;

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
    enum MusicCategoryData
    {
        IconKey = Qt::UserRole,
        MusicCount,
        VariousArtist,
        ArtistName,
        Year
    };

    virtual void updateImage(const int &index);
    virtual QIcon itemIcon(const int &index) const;
    virtual QString categoryName(const int &index) const;
    KNMusicModel *m_sourceModel;
    int m_noCategoryItemCount=0;
    QIcon m_noAlbumArtIcon;

private slots:
    void updateAllImage();

private:
    QString m_noCategoryText;
};

#endif // KNMUSICCATEGORYMODEL_H
