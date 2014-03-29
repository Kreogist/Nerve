#include <QDebug>

#include <QStandardItem>

#include "../knmusicglobal.h"

#include "knmusicgenremodel.h"

KNMusicGenreModel::KNMusicGenreModel(QObject *parent) :
    KNMusicCategoryModel(parent)
{
    retranslateAndSet();
    m_musicGlobal=KNMusicGlobal::instance();
}

void KNMusicGenreModel::retranslate()
{
    setNoCategoryText(tr("No Genre"));
}

void KNMusicGenreModel::retranslateAndSet()
{
    retranslate();
    QStandardItem *noArtistItem=item(0);
    noArtistItem->setText(noCategoryText());
}

QIcon KNMusicGenreModel::itemIcon(const int &index) const
{
    QPixmap genreArt=m_musicGlobal->getGenreImage(m_sourceModel->item(index, KNMusicGlobal::Genre)->text());
    if(genreArt.isNull())
    {
        return KNMusicCategoryModel::itemIcon(index);
    }
    return QIcon(genreArt);
}

QString KNMusicGenreModel::categoryName(const int &index) const
{
    return m_sourceModel->item(index,
                               KNMusicGlobal::Genre)->data(Qt::DisplayRole).toString();
}
