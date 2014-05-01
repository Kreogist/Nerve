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
    QPixmap albumArt=m_sourceModel->item(index,
                                         KNMusicGlobal::Time)->data(Qt::UserRole+1).value<QPixmap>();
    if(albumArt.isNull())
    {
        return KNMusicCategoryModel::itemIcon(index);
    }
    return QIcon(albumArt);
}

QString KNMusicArtistModel::categoryName(const int &index) const
{
    return m_sourceModel->item(index,
                               KNMusicGlobal::Artist)->data(Qt::DisplayRole).toString();
}
