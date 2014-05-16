#include <QDebug>

#include <QStandardItem>

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

QIcon KNMusicArtistModel::itemIcon(const int &index) const
{
    QPixmap albumArt=QPixmap::fromImage(m_sourceModel->itemArtwork(index));
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
