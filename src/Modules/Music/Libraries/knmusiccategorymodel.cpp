#include <QDebug>

#include "knmusicartistitem.h"

#include "../knmusicglobal.h"

#include "knmusiccategorymodel.h"

KNMusicCategoryModel::KNMusicCategoryModel(QObject *parent) :
    QStandardItemModel(parent)
{
    retranslate();

    m_noAlbumArtIcon=QIcon(QPixmap(":/Music/Resources/Music/noalbum.png"));
    setSortRole(Qt::UserRole);

    resetModel();
}

KNMusicArtistItem *KNMusicCategoryModel::artistItem(const QModelIndex &index)
{
    return static_cast<KNMusicArtistItem *>(itemFromIndex(index));
}

void KNMusicCategoryModel::resetModel()
{
    clear();
    KNMusicArtistItem *currentCategory=new KNMusicArtistItem(m_noCategoryText);
    currentCategory->setIcon(m_noAlbumArtIcon);
    appendRow(currentCategory);
}

QString KNMusicCategoryModel::noCategoryText() const
{
    return m_noCategoryText;
}

void KNMusicCategoryModel::retranslate()
{
    ;
}

void KNMusicCategoryModel::retranslateAndSet()
{
    retranslate();
}

void KNMusicCategoryModel::onMusicAdded(const QModelIndex &index)
{
    QString currentName=categoryName(index.row());
    KNMusicArtistItem *currentItem;
    if(currentName.isEmpty())
    {
        m_noCategoryItemCount++;
        emit requireShowFirstItem();
        return;
    }
    QList<QStandardItem *> searchResult=findItems(currentName);
    if(searchResult.size()==0)
    {
        currentItem=new KNMusicArtistItem(currentName);
        currentItem->setData(currentName, Qt::UserRole);
        currentItem->setData(1, Qt::UserRole+1);
        searchResult.append(currentItem);
        appendRow(currentItem);
    }
    else
    {
        currentItem=static_cast<KNMusicArtistItem *>(searchResult.first());
        currentItem->setData(currentItem->data(Qt::UserRole+1).toInt()+1,
                             Qt::UserRole+1);
    }
}

void KNMusicCategoryModel::onMusicRecover(const QModelIndex &index)
{
    QString currentName=categoryName(index.row());
    KNMusicArtistItem *currentItem;
    if(currentName.isEmpty())
    {
        m_noCategoryItemCount++;
        emit requireShowFirstItem();
        return;
    }
    QList<QStandardItem *> searchResult=findItems(currentName);
    if(searchResult.size()==0)
    {
        currentItem=new KNMusicArtistItem(currentName);
        currentItem->setData(currentName, Qt::UserRole);
        currentItem->setData(1, Qt::UserRole+1);
        searchResult.append(currentItem);
        appendRow(currentItem);
    }
    else
    {
        currentItem=static_cast<KNMusicArtistItem *>(searchResult.first());
        currentItem->setData(currentItem->data(Qt::UserRole+1).toInt()+1,
                             Qt::UserRole+1);
        currentItem->setIcon(itemIcon(index.row()));
    }
}

void KNMusicCategoryModel::onMusicRemoved(const QModelIndex &index)
{
    QString currentName=categoryName(index.row());
    KNMusicArtistItem *currentItem;
    if(currentName.isEmpty())
    {
        m_noCategoryItemCount--;
        if(m_noCategoryItemCount==0)
        {
            emit requireHideFirstItem();
        }
        return;
    }
    QList<QStandardItem *> searchResult=findItems(currentName);
    if(searchResult.size()==0)
    {
        return;
    }
    currentItem=static_cast<KNMusicArtistItem *>(searchResult.first());
    int currentItemSize=currentItem->data(Qt::UserRole+1).toInt();
    if(currentItemSize==1)
    {
        removeRow(currentItem->row());
    }
    else
    {
        currentItem->setData(currentItemSize-1, Qt::UserRole+1);
    }
}

void KNMusicCategoryModel::onAlbumArtUpdate(const int &index)
{
    QString currentName=categoryName(index);
    if(currentName.isEmpty())
    {
        return;
    }
    QList<QStandardItem *> searchResult=findItems(currentName);
    if(searchResult.size()==0)
    {
        return;
    }
    KNMusicArtistItem *currentItem=
            static_cast<KNMusicArtistItem *>(searchResult.first());
    if(currentItem->hasIcon())
    {
        return;
    }
    currentItem->setIcon(itemIcon(index));
    currentItem->setHasIcon(true);
}

void KNMusicCategoryModel::setSourceModel(QAbstractItemModel *sourceModel)
{
    KNMusicModel *musicModel=qobject_cast<KNMusicModel *>(sourceModel);
    m_sourceModel=musicModel;
    connect(m_sourceModel, &KNMusicModel::musicAppend,
            this, &KNMusicCategoryModel::onMusicAdded);
    connect(m_sourceModel, &KNMusicModel::musicRecover,
            this, &KNMusicCategoryModel::onMusicRecover);
    connect(m_sourceModel, &KNMusicModel::musicAlbumArtUpdate,
            this, &KNMusicCategoryModel::onAlbumArtUpdate);
}

QIcon KNMusicCategoryModel::itemIcon(const int &index) const
{
    Q_UNUSED(index);
    return m_noAlbumArtIcon;
}

QString KNMusicCategoryModel::categoryName(const int &index) const
{
    Q_UNUSED(index);
    return QString();
}

void KNMusicCategoryModel::setNoCategoryText(const QString &noCategoryText)
{
    m_noCategoryText = noCategoryText;
}
