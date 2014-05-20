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
    //Clear the QList
    m_textList.clear();
    m_detailList.clear();
    m_extraList.clear();

    //Initial the first no category text item.
    MusicCategoryItem currentCategory;
    currentCategory.decoration=m_noAlbumArtIcon;
    currentCategory.songCount=0;
    AlbumExtras currentExtras;
    currentExtras.variousArtist=false;
    insertAlbum("", currentCategory, currentExtras);
}

QString KNMusicAlbumModel::indexArtist(const QModelIndex &index) const
{
    return m_extraList.at(index.row()).artist;
}

QString KNMusicAlbumModel::indexYear(const QModelIndex &index) const
{
    return m_extraList.at(index.row()).year;
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
    QString currentArtist=artistFromSource(index.row());
    MusicCategoryItem currentAlbum;
    AlbumExtras currentAlbumExtras;
    if(searchResult==-1)
    {
        currentAlbum.songCount=1;
        currentAlbumExtras.artist=currentArtist;
        currentAlbumExtras.variousArtist=false;
        insertAlbum(currentName, currentAlbum, currentAlbumExtras);
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
    QString currentArtist=artistFromSource(index.row()),
            currentYear=yearFromSource(index.row());
    if(searchResult==-1)
    {
        currentAlbum.songCount=1;
        currentAlbum.iconKey=m_sourceModel->itemArtworkKey(index.row());
        currentAlbumExtras.artist=currentArtist;
        currentAlbumExtras.variousArtist=false;
        currentAlbumExtras.year=currentYear;
        insertAlbum(currentName, currentAlbum, currentAlbumExtras);
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
        if(currentAlbumExtras.year.isEmpty() &&
                !currentYear.isEmpty())
        {
            currentAlbumExtras.year=currentYear;
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

void KNMusicAlbumModel::insertAlbum(const QString &text,
                                    const KNMusicCategoryModel::MusicCategoryItem &details,
                                    const KNMusicAlbumModel::AlbumExtras &extras)
{
    //emit beginInsertRows(QModelIndex(), m_textList.size(), m_textList.size()+1);
    m_textList.append(text);
    m_detailList.append(details);
    m_extraList.append(extras);
    //emit endInsertRows();
}

QIcon KNMusicAlbumModel::itemIcon(const int &index) const
{
    QPixmap albumArt=QPixmap::fromImage(m_sourceModel->artwork(index));
    return albumArt.isNull()?KNMusicCategoryModel::itemIcon(index):QIcon(albumArt);
}

QString KNMusicAlbumModel::artistFromSource(const int &index) const
{
    return m_sourceModel->itemText(index, KNMusicGlobal::Artist);
}

QString KNMusicAlbumModel::yearFromSource(const int &index) const
{
    return m_sourceModel->itemText(index, KNMusicGlobal::Year);
}

void KNMusicAlbumModel::onActionRemoveRow(const int &index)
{
    m_extraList.removeAt(index);
}

QString KNMusicAlbumModel::categoryName(const int &index) const
{
    return m_sourceModel->itemText(index, KNMusicGlobal::Album);
}
