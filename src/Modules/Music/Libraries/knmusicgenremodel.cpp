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
    setData(index(0,0), noCategoryText(), Qt::DisplayRole);
}

void KNMusicGenreModel::updateImage(const int &index)
{
    MusicCategoryItem currentItem=m_detailList.at(index);
    QPixmap genreImage=m_musicGlobal->getGenreImage(m_textList.at(index));
    currentItem.decoration=genreImage.isNull()?m_noAlbumArtIcon:genreImage;
    m_detailList.replace(index, currentItem);
}

QIcon KNMusicGenreModel::itemIcon(const int &index) const
{
    QPixmap genreArt=m_musicGlobal->getGenreImage(m_sourceModel->item(index,
                                                                      KNMusicGlobal::Genre)->text());
    return genreArt.isNull()?KNMusicCategoryModel::itemIcon(index):QIcon(genreArt);
}

QString KNMusicGenreModel::categoryName(const int &index) const
{
    return m_sourceModel->item(index,
                               KNMusicGlobal::Genre)->data(Qt::DisplayRole).toString();
}
