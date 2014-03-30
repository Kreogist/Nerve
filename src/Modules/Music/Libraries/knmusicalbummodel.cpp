#include "knmusicalbummodel.h"

KNMusicAlbumModel::KNMusicAlbumModel(QObject *parent) :
    KNMusicCategoryModel(parent)
{
    retranslateAndSet();
}

void KNMusicAlbumModel::retranslate()
{
    setNoCategoryText(tr("No Album"));
}

void KNMusicAlbumModel::retranslateAndSet()
{
    retranslate();
    QStandardItem *noArtistItem=item(0);
    noArtistItem->setText(noCategoryText());
}

QIcon KNMusicAlbumModel::itemIcon(const int &index) const
{
    QPixmap albumArt=m_sourceModel->item(index, KNMusicGlobal::Time)->data(Qt::UserRole).value<QPixmap>();
    if(albumArt.isNull())
    {
        return KNMusicCategoryModel::itemIcon(index);
    }
    return QIcon(albumArt);
}

QString KNMusicAlbumModel::categoryName(const int &index) const
{
    return m_sourceModel->item(index,
                               KNMusicGlobal::Album)->data(Qt::DisplayRole).toString();
}
