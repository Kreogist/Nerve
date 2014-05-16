#include <QDebug>

#include "knmusicartistitem.h"
#include "../knmusicglobal.h"

#include "knmusicartistmodel.h"

KNMusicArtistModel::KNMusicArtistModel(QObject *parent) :
    KNMusicCategoryModel(parent)
{
    retranslateAndSet();
}

void KNMusicArtistModel::retranslate()
{
    setNoCategoryText(tr("No Artist"));
}

void KNMusicArtistModel::retranslateAndSet()
{
    retranslate();
    QStandardItem *noArtistItem=item(0);
    noArtistItem->setText(noCategoryText());
}

void KNMusicArtistModel::updateImage(const int &index)
{
    KNMusicArtistItem *currentItem=
            static_cast<KNMusicArtistItem *>(item(index));
    QString iconKey=currentItem->iconKey();
    if(iconKey.isEmpty())
    {
        currentItem->setIcon(m_noAlbumArtIcon);
    }
    else
    {
        currentItem->setIcon(QPixmap::fromImage(m_sourceModel->artworkFromKey(iconKey)));
    }
}

QIcon KNMusicArtistModel::itemIcon(const int &index) const
{
    QPixmap albumArt=QPixmap::fromImage(m_sourceModel->artwork(index));
    if(albumArt.isNull())
    {
        return KNMusicCategoryModel::itemIcon(index);
    }
    return QIcon(albumArt);
}

QString KNMusicArtistModel::categoryName(const int &index) const
{
    return m_sourceModel->itemText(index, KNMusicGlobal::Artist);
}
