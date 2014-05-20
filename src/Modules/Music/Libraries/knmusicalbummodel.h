#ifndef KNMUSICALBUMMODEL_H
#define KNMUSICALBUMMODEL_H

#include "knmusiccategorymodel.h"

class KNMusicAlbumModel : public KNMusicCategoryModel
{
    Q_OBJECT
public:
    explicit KNMusicAlbumModel(QObject *parent = 0);
    bool isNoAlbumHidden() const;
    void resetModel();
    QString indexArtist(const QModelIndex &index) const;
    QString indexYear(const QModelIndex &index) const;

signals:
    void albumRemoved(const QModelIndex &index);

public slots:
    void retranslate();
    void retranslateAndSet();
    void onMusicAdded(const QModelIndex &index);
    void onMusicRemoved(const QModelIndex &index);
    void onMusicRecover(const QModelIndex &index);

protected:
    struct AlbumExtras
    {
        QString artist;
        bool variousArtist;
        QString year;
    };
    void updateImage(const int &index);
    void insertAlbum(const QString &text,
                     const MusicCategoryItem &details,
                     const AlbumExtras &extras);
    QIcon itemIcon(const int &index) const;
    QString categoryName(const int &index) const;
    QString artistFromSource(const int &index) const;
    QString yearFromSource(const int &index) const;
    void onActionRemoveRow(const int &index);

private:
    QList<AlbumExtras> m_extraList;
    QString m_variousArtist;
};

#endif // KNMUSICALBUMMODEL_H
