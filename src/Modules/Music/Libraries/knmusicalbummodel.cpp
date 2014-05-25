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
    return data(index(0,0), MusicCount).toInt()==0;
}

void KNMusicAlbumModel::resetModel()
{
    //Clear the model
    clear();

    //Initial the first no category text item.
    QStandardItem *currentItem=new QStandardItem();
    currentItem->setData(noCategoryText(), Qt::DisplayRole);
    currentItem->setData(m_noAlbumArtIcon, Qt::DecorationRole);
    currentItem->setData(0, MusicCount);
    currentItem->setData(false, VariousArtist);
    currentItem->setData("", ArtistName);
    appendRow(currentItem);
}

QString KNMusicAlbumModel::indexArtist(const QModelIndex &index) const
{
    return data(index, ArtistName).toString();
}

QString KNMusicAlbumModel::indexYear(const QModelIndex &index) const
{
    return data(index, Year).toString();
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
    QModelIndex searchResult;
    if(currentName.isEmpty())
    {
        searchResult=this->index(0,0);
        setData(searchResult, data(searchResult, MusicCount).toInt()+1, MusicCount);
        emit requireShowFirstItem();
        return;
    }
    searchResult=indexOf(currentName);
    QString currentArtist=artistFromSource(index.row());
    QStandardItem *currentAlbum;
    if(searchResult.isValid())
    {
        currentAlbum=itemFromIndex(searchResult);
        currentAlbum->setData(currentAlbum->data(MusicCount).toInt()+1, MusicCount);
        if(currentAlbum->data(VariousArtist)==false &&
           currentAlbum->data(ArtistName).toString()!=currentArtist)
        {
            currentAlbum->setData(true, VariousArtist);
            currentAlbum->setData(m_variousArtist, ArtistName);
        }
        if(currentAlbum->data(Year).toString().isEmpty())
        {
            currentAlbum->setData(yearFromSource(index.row()));
        }
    }
    else
    {
        currentAlbum=new QStandardItem(currentName);
        currentAlbum->setData(m_noAlbumArtIcon, Qt::DecorationRole);
        currentAlbum->setData(1, MusicCount);
        currentAlbum->setData(currentArtist, ArtistName);
        currentAlbum->setData(false, VariousArtist);
        appendRow(currentAlbum);
    }
}

void KNMusicAlbumModel::onMusicRemoved(const QModelIndex &index)
{
    QString currentName=categoryName(index.row());
    QModelIndex searchResult;
    if(currentName.isEmpty())
    {
        searchResult=this->index(0,0);
        setData(searchResult, data(searchResult, MusicCount).toInt()-1, MusicCount);
        if(data(searchResult, MusicCount).toInt()==0)
        {
            emit requireHideFirstItem();
        }
        return;
    }
    searchResult=indexOf(currentName);
    if(!searchResult.isValid())
    {
        return;
    }
    QStandardItem *currentAlbum=itemFromIndex(searchResult);
    int currentMusicCount=currentAlbum->data(MusicCount).toInt();
    if(currentMusicCount==1)
    {
        emit albumRemoved(searchResult);
        removeRow(searchResult.row());
    }
    else
    {
        currentAlbum->setData(currentMusicCount-1, MusicCount);
    }
}

void KNMusicAlbumModel::onMusicRecover(const QModelIndex &index)
{
    QString currentName=categoryName(index.row());
    QModelIndex searchResult;
    if(currentName.isEmpty())
    {
        searchResult=this->index(0,0);
        setData(searchResult, data(searchResult, MusicCount).toInt()+1, MusicCount);
        emit requireShowFirstItem();
        return;
    }
    searchResult=indexOf(currentName);
    QString currentArtist=artistFromSource(index.row());
    QStandardItem *currentAlbum;
    if(searchResult.isValid())
    {
        currentAlbum=itemFromIndex(searchResult);
        currentAlbum->setData(currentAlbum->data(MusicCount).toInt()+1, MusicCount);
        if(currentAlbum->data(VariousArtist)==false &&
           currentAlbum->data(ArtistName).toString()!=currentArtist)
        {
            currentAlbum->setData(true, VariousArtist);
            currentAlbum->setData(m_variousArtist, ArtistName);
        }
        if(currentAlbum->data(Year).toString().isEmpty())
        {
            currentAlbum->setData(yearFromSource(index.row()));
        }
    }
    else
    {
        currentAlbum=new QStandardItem(currentName);
        currentAlbum->setData(m_sourceModel->itemArtworkKey(index.row()), IconKey);
        currentAlbum->setData(1, MusicCount);
        currentAlbum->setData(currentArtist, ArtistName);
        currentAlbum->setData(false, VariousArtist);
        appendRow(currentAlbum);
    }
}

void KNMusicAlbumModel::updateImage(const int &index)
{
    QStandardItem *currentItem=item(index);
    QString currentIconKey=currentItem->data(IconKey).toString();
    currentItem->setData(currentIconKey.isEmpty()?
                             m_noAlbumArtIcon:
                             QPixmap::fromImage(m_sourceModel->artworkFromKey(currentIconKey)),
                         Qt::DecorationRole);
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

QString KNMusicAlbumModel::categoryName(const int &index) const
{
    return m_sourceModel->itemText(index, KNMusicGlobal::Album);
}
