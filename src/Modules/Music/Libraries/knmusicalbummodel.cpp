#include <QDebug>

#include "knmusicartistitem.h"

#include "knmusicalbummodel.h"

KNMusicAlbumModel::KNMusicAlbumModel(QObject *parent) :
    KNMusicCategoryModel(parent)
{
    retranslateAndSet();
}

bool KNMusicAlbumModel::isNoAlbumHidden() const
{
    return (m_noCategoryItemCount==0);
}

void KNMusicAlbumModel::retranslate()
{
    setNoCategoryText(tr("No Album"));
    m_variousArtist=tr("Various Artists");
}

void KNMusicAlbumModel::retranslateAndSet()
{
    retranslate();
    QStandardItem *noArtistItem=item(0);
    noArtistItem->setText(noCategoryText());
}

void KNMusicAlbumModel::onMusicAdded(const QModelIndex &index)
{
    QString currentName=categoryName(index.row());
    KNMusicArtistItem *currentAlbum;
    if(currentName.isEmpty())
    {
        m_noCategoryItemCount++;
        emit requireShowFirstItem();
        return;
    }
    QList<QStandardItem *> searchResult=findItems(currentName);
    QString currentArtist=artistName(index.row());
    if(searchResult.size()==0)
    {
        currentAlbum=new KNMusicArtistItem(currentName);
        currentAlbum->setData(currentName, Qt::DisplayRole);
        currentAlbum->setIcon(itemIcon(index.row()));
        currentAlbum->setData(currentArtist, Qt::UserRole);
        currentAlbum->setData(0, Qt::UserRole+1);
        currentAlbum->setData(1, Qt::UserRole+2);
        searchResult.append(currentAlbum);
        appendRow(currentAlbum);
    }
    else
    {
        currentAlbum=static_cast<KNMusicArtistItem *>(searchResult.first());
        if(currentAlbum->data(Qt::UserRole+1).toInt() == 0 &&
           currentAlbum->data(Qt::UserRole).toString() != currentArtist)
        {
            currentAlbum->setData(m_variousArtist, Qt::UserRole);
            currentAlbum->setData(1, Qt::UserRole+1);
            currentAlbum->setData(currentAlbum->data(Qt::UserRole+2).toInt()+1,
                                  Qt::UserRole+2);
        }
    }
}

void KNMusicAlbumModel::onMusicRemoved(const QModelIndex &index)
{
    QString currentName=categoryName(index.row());
    if(currentName.isEmpty())
    {
        m_noCategoryItemCount--;
        emit requireHideFirstItem();
        return;
    }
    KNMusicArtistItem *currentAlbum;
    QList<QStandardItem *> searchResult=findItems(currentName);
    if(searchResult.size()==0)
    {
        return;
    }
    currentAlbum=static_cast<KNMusicArtistItem *>(searchResult.first());
    int currentAlbumSong=currentAlbum->data(Qt::UserRole+2).toInt();
    if(currentAlbumSong==1)
    {
        removeColumn(currentAlbum->row());
    }
    else
    {
        currentAlbum->setData(currentAlbumSong-1, Qt::UserRole+2);
    }
}

QIcon KNMusicAlbumModel::itemIcon(const int &index) const
{
    QPixmap albumArt=m_sourceModel->item(index,
                                         KNMusicGlobal::Time)->data(Qt::UserRole+1).value<QPixmap>();
    if(albumArt.isNull())
    {
        return KNMusicCategoryModel::itemIcon(index);
    }
    return QIcon(albumArt);
}

QString KNMusicAlbumModel::artistName(const int &index) const
{
    return m_sourceModel->item(index,
                               KNMusicGlobal::Artist)->data(Qt::DisplayRole).toString();
}

QString KNMusicAlbumModel::categoryName(const int &index) const
{
    return m_sourceModel->item(index,
                               KNMusicGlobal::Album)->data(Qt::DisplayRole).toString();
}
