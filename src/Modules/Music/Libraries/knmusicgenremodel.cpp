#include <QDebug>

#include <QStandardItem>

#include "../knmusicglobal.h"

#include "knmusicgenremodel.h"

KNMusicGenreModel::KNMusicGenreModel(QObject *parent) :
    KNMusicCategoryModel(parent)
{
    retranslateAndSet();
}

void KNMusicGenreModel::retranslate()
{
    setNoCategoryText(tr("Unknown Genre"));
}

void KNMusicGenreModel::retranslateAndSet()
{
    retranslate();
    QStandardItem *noArtistItem=item(0);
    noArtistItem->setText(noCategoryText());
}

QIcon KNMusicGenreModel::itemIcon(const int &index) const
{
    QPixmap albumArt=m_sourceModel->item(index, KNMusicGlobal::Time)->data(Qt::UserRole).value<QPixmap>();
    if(albumArt.isNull())
    {
        return KNMusicCategoryModel::itemIcon(index);
    }
    return QIcon(albumArt);
}

QString KNMusicGenreModel::categoryName(const int &index) const
{
    return m_sourceModel->item(index,
                               KNMusicGlobal::Genre)->data(Qt::DisplayRole).toString();
}
