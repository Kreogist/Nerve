#include <QDebug>

#include "knmusicartistitem.h"

#include "knmusicalbummodel.h"

KNMusicAlbumModel::KNMusicAlbumModel(QObject *parent) :
    KNMusicCategoryModel(parent)
{
    retranslateAndSet();
    resetModel();
}

bool KNMusicAlbumModel::isNoAlbumHidden() const
{
    return (m_noCategoryItemCount==0);
}

void KNMusicAlbumModel::resetModel()
{
    KNMusicCategoryModel::resetModel();
    m_extraList.clear();

    AlbumExtras currentExtras;
    currentExtras.variousArtist=false;
    m_extraList.append(currentExtras);
}

QString KNMusicAlbumModel::indexArtist(const QModelIndex &index) const
{
    return m_extraList.at(index.row()).artist;
}

void KNMusicAlbumModel::retranslate()
{
    setNoCategoryText(tr("No Album"));
    m_variousArtist=tr("Various Artists");
}

void KNMusicAlbumModel::retranslateAndSet()
{
    retranslate();
    setData(index(0,0), noCategoryText(), Qt::DisplayRole);
}

void KNMusicAlbumModel::onMusicAdded(const QModelIndex &index)
{
    QString currentName=categoryName(index.row());
    if(currentName.isEmpty())
    {
        m_noCategoryItemCount++;
        emit requireShowFirstItem();
        return;
    }
    int searchResult=m_textList.indexOf(currentName);
    MusicCategoryItem currentAlbum;
    AlbumExtras currentAlbumExtras;
    QString currentArtist=artistName(index.row());
    if(searchResult==-1)
    {
        m_textList.append(currentName);
        currentAlbum.songCount=1;
        m_detailList.append(currentAlbum);
        currentAlbumExtras.artist=currentArtist;
        currentAlbumExtras.variousArtist=false;
        m_extraList.append(currentAlbumExtras);
    }
    else
    {
        currentAlbum=m_detailList.at(searchResult);
        currentAlbum.songCount++;
        m_detailList.replace(searchResult, currentAlbum);
        currentAlbumExtras=m_extraList.at(searchResult);
        if(currentAlbumExtras.variousArtist==false &&
           currentAlbumExtras.artist!=currentArtist)
        {
            currentAlbumExtras.artist=m_variousArtist;
            currentAlbumExtras.variousArtist=true;
            m_extraList.replace(searchResult, currentAlbumExtras);
        }
    }
}

void KNMusicAlbumModel::onMusicRemoved(const QModelIndex &index)
{
    QString currentName=categoryName(index.row());
    if(currentName.isEmpty())
    {
        m_noCategoryItemCount--;
        if(m_noCategoryItemCount==0)
        {
            emit requireHideFirstItem();
        }
        return;
    }
    int searchResult=m_textList.indexOf(currentName);
    if(searchResult==-1)
    {
        return;
    }
    MusicCategoryItem currentAlbum=m_detailList.at(searchResult);
    if(currentAlbum.songCount==1)
    {
        emit albumRemoved(this->index(searchResult, 0));
        removeRow(searchResult);
    }
    else
    {
        currentAlbum.songCount--;
        m_detailList.replace(searchResult, currentAlbum);
    }
}

void KNMusicAlbumModel::onMusicRecover(const QModelIndex &index)
{
    QString currentName=categoryName(index.row());
    if(currentName.isEmpty())
    {
        m_noCategoryItemCount++;
        emit requireShowFirstItem();
        return;
    }
    int searchResult=m_textList.indexOf(currentName);
    MusicCategoryItem currentAlbum;
    AlbumExtras currentAlbumExtras;
    QString currentArtist=artistName(index.row());
    if(searchResult==-1)
    {
        m_textList.append(currentName);
        currentAlbum.songCount=1;
        currentAlbum.iconKey=m_sourceModel->itemArtworkKey(index.row());
        m_detailList.append(currentAlbum);
        currentAlbumExtras.artist=currentArtist;
        currentAlbumExtras.variousArtist=false;
        m_extraList.append(currentAlbumExtras);
    }
    else
    {
        currentAlbum=m_detailList.at(searchResult);
        currentAlbum.songCount++;
        m_detailList.replace(searchResult, currentAlbum);
        currentAlbumExtras=m_extraList.at(searchResult);
        if(currentAlbumExtras.variousArtist==false &&
           currentAlbumExtras.artist!=currentArtist)
        {
            currentAlbumExtras.artist=m_variousArtist;
            currentAlbumExtras.variousArtist=true;
            m_extraList.replace(searchResult, currentAlbumExtras);
        }
    }
}

void KNMusicAlbumModel::updateImage(const int &index)
{
    MusicCategoryItem currentItem=m_detailList.at(index);
    currentItem.decoration=currentItem.iconKey.isEmpty()?
                           m_noAlbumArtIcon:
                           QPixmap::fromImage(m_sourceModel->artworkFromKey(currentItem.iconKey));
    m_detailList.replace(index, currentItem);
}

QIcon KNMusicAlbumModel::itemIcon(const int &index) const
{
    QPixmap albumArt=QPixmap::fromImage(m_sourceModel->artwork(index));
    return albumArt.isNull()?KNMusicCategoryModel::itemIcon(index):QIcon(albumArt);
}

QString KNMusicAlbumModel::artistName(const int &index) const
{
    return m_sourceModel->itemText(index, KNMusicGlobal::Artist);
}

void KNMusicAlbumModel::onActionRemoveRow(const int &index)
{
    m_extraList.removeAt(index);
}

QString KNMusicAlbumModel::categoryName(const int &index) const
{
    return m_sourceModel->itemText(index, KNMusicGlobal::Album);
}
