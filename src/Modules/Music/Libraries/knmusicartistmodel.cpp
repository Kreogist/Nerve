#include <QDebug>

#include "knmusicmodel.h"
#include "knmusicartistitem.h"

#include "../knmusicglobal.h"

#include "knmusicartistmodel.h"

KNMusicArtistModel::KNMusicArtistModel(QObject *parent) :
    QStandardItemModel(parent)
{
    retranslate();

    m_noAlbumIcon=QIcon(QPixmap(":/Music/Resources/Music/noalbum.png"));
    setSortRole(Qt::UserRole);

    resetModel();
}

KNMusicArtistItem *KNMusicArtistModel::artistItem(const QModelIndex &index)
{
    return static_cast<KNMusicArtistItem *>(itemFromIndex(index));
}

void KNMusicArtistModel::resetModel()
{
    clear();
    KNMusicArtistItem *currentArtist=new KNMusicArtistItem(m_noArtist);
    currentArtist->setIcon(m_noAlbumIcon);
    appendRow(currentArtist);
}

void KNMusicArtistModel::retranslate()
{
    m_noArtist=tr("No Artist");
}

void KNMusicArtistModel::retranslateAndSet()
{
    retranslate();
}

void KNMusicArtistModel::onMusicAdded(const QModelIndex &index)
{
    QString artistName=m_sourceModel->item(index.row(),
                                           KNMusicGlobal::Artist)->data(Qt::DisplayRole).toString();
    KNMusicArtistItem *currentArtist;
    if(artistName.isEmpty())
    {
        return;
    }
    QList<QStandardItem *> searchResult=findItems(artistName);
    if(searchResult.size()==0)
    {
        currentArtist=new KNMusicArtistItem(artistName);
        currentArtist->setData(artistName, Qt::UserRole);
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
}

void KNMusicArtistModel::setSourceModel(QAbstractItemModel *sourceModel)
{
    KNMusicModel *musicModel=qobject_cast<KNMusicModel *>(sourceModel);
    m_sourceModel=musicModel;
    connect(m_sourceModel, &KNMusicModel::musicAppend,
            this, &KNMusicArtistModel::onMusicAdded);
}
