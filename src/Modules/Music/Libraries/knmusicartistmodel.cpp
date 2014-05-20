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
    setData(index(0,0), noCategoryText(), Qt::DisplayRole);
}

void KNMusicArtistModel::updateImage(const int &index)
{
    QStandardItem *currentItem=item(index);
    QString currentIconKey=currentItem->data(IconKey).toString();
    currentItem->setData(currentIconKey.isEmpty()?
                             m_noAlbumArtIcon:
                             QPixmap::fromImage(m_sourceModel->artworkFromKey(currentIconKey)),
                         Qt::DecorationRole);
}

QIcon KNMusicArtistModel::itemIcon(const int &index) const
{
    QPixmap albumArt=QPixmap::fromImage(m_sourceModel->artwork(index));
    return albumArt.isNull()?KNMusicCategoryModel::itemIcon(index):QIcon(albumArt);
}

QString KNMusicArtistModel::categoryName(const int &index) const
{
    return m_sourceModel->itemText(index, KNMusicGlobal::Artist);
}
