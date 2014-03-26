#include <QDebug>

#include "knmusicmodel.h"
#include "knmusicartistitem.h"

#include "../knmusicglobal.h"

#include "knmusicartistmodel.h"

KNMusicArtistModel::KNMusicArtistModel(QObject *parent) :
    QStandardItemModel(parent)
{
    m_noAlbumIcon=QIcon(QPixmap(":/Music/Resources/Music/noalbum.png"));
}

KNMusicArtistItem *KNMusicArtistModel::artistItem(const QModelIndex &index)
{
    return static_cast<KNMusicArtistItem *>(itemFromIndex(index));
}

void KNMusicArtistModel::onMusicAdded(const QModelIndex &index)
{
    QString artistName=m_sourceModel->item(index.row(),
                                           KNMusicGlobal::Artist)->data(Qt::DisplayRole).toString();
    QList<QStandardItem *> searchResult=findItems(artistName);
    KNMusicArtistItem *currentArtist;
    if(searchResult.size()==0)
    {
        currentArtist=new KNMusicArtistItem(artistName);
        currentArtist->addSongs(index);
        QPixmap albumArt=m_sourceModel->item(index.row(),KNMusicGlobal::Time)->data(Qt::UserRole).value<QPixmap>();
        if(albumArt.isNull())
        {
            currentArtist->setIcon(m_noAlbumIcon);
        }
        else
        {
            currentArtist->setIcon(QIcon(albumArt));
        }
        searchResult.append(currentArtist);
        appendRow(currentArtist);
    }
    else
    {
        currentArtist=static_cast<KNMusicArtistItem *>(searchResult.at(0));
        currentArtist->addSongs(index);
    }
}

void KNMusicArtistModel::setSourceModel(QAbstractItemModel *sourceModel)
{
    KNMusicModel *musicModel=qobject_cast<KNMusicModel *>(sourceModel);
    m_sourceModel=musicModel;
    connect(m_sourceModel, &KNMusicModel::musicAppend,
            this, &KNMusicArtistModel::onMusicAdded);
}
