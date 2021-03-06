#include <QDebug>

#include "../knmusicglobal.h"

#include "knmusiccategorymodel.h"

KNMusicCategoryModel::KNMusicCategoryModel(QObject *parent) :
    QStandardItemModel(parent)
{
    retranslate();

    m_noAlbumArtIcon=QIcon(QPixmap::fromImage(KNMusicGlobal::instance()->noAlbumImage()));

    resetModel();
}

QModelIndex KNMusicCategoryModel::indexOf(const QString &text) const
{
    QList<QStandardItem *> items=findItems(text);
    if(items.isEmpty())
    {
        return QModelIndex();
    }
    return items.first()->index();
}

QModelIndex KNMusicCategoryModel::firstItemIndex() const
{
    if(rowCount()==1)
    {
        return data(index(0,0), MusicCount).toInt()==0?QModelIndex():index(0,0);
    }
    return data(index(0,0), MusicCount).toInt()==0?index(1,0):index(0,0);
}

void KNMusicCategoryModel::resetModel()
{
    //Clear the model
    clear();

    //Add initial item
    QStandardItem *currentItem=new QStandardItem();
    currentItem->setData(noCategoryText(), Qt::DisplayRole);
    currentItem->setData(m_noAlbumArtIcon, Qt::DecorationRole);
    currentItem->setEditable(false);
    currentItem->setData(0, MusicCount);
    appendRow(currentItem);
}

QString KNMusicCategoryModel::filterText(const QModelIndex &index) const
{
    return data(index, Qt::DisplayRole).toString();
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
    QModelIndex searchResult;
    if(currentName.isEmpty())
    {
        searchResult=this->index(0,0);
        setData(searchResult, data(searchResult, MusicCount).toInt()+1, MusicCount);
        return;
    }
    searchResult=indexOf(currentName);
    QStandardItem *currentItem;
    if(searchResult.isValid())
    {
        setData(searchResult, data(searchResult, MusicCount).toInt()+1, MusicCount);
    }
    else
    {
        currentItem=new QStandardItem(currentName);
        currentItem->setData(m_noAlbumArtIcon, Qt::DecorationRole);
        currentItem->setEditable(false);
        currentItem->setData(1, MusicCount);
        appendRow(currentItem);
    }
}

void KNMusicCategoryModel::onMusicRecover(const QModelIndex &index)
{
    QString currentName=categoryName(index.row());
    QModelIndex searchResult;
    if(currentName.isEmpty())
    {
        searchResult=this->index(0,0);
        setData(searchResult, data(searchResult, MusicCount).toInt()+1, MusicCount);
        return;
    }
    searchResult=indexOf(currentName);
    QStandardItem *currentItem;
    if(searchResult.isValid())
    {
        setData(searchResult, data(searchResult, MusicCount).toInt()+1, MusicCount);
    }
    else
    {
        currentItem=new QStandardItem(currentName);
        currentItem->setData(m_sourceModel->artworkKey(index.row()), IconKey);
        currentItem->setEditable(false);
        currentItem->setData(1, MusicCount);
        appendRow(currentItem);
    }
}

void KNMusicCategoryModel::onMusicRemoved(const QModelIndex &index)
{
    QString currentName=categoryName(index.row());
    QModelIndex searchResult;
    if(currentName.isEmpty())
    {
        searchResult=this->index(0,0);
        setData(searchResult, data(searchResult, MusicCount).toInt()-1, MusicCount);
        return;
    }
    searchResult=indexOf(currentName);
    if(!searchResult.isValid())
    {
        return;
    }
    QStandardItem *currentItem=itemFromIndex(searchResult);
    int currentMusicCount=currentItem->data(MusicCount).toInt();
    if(currentMusicCount==1)
    {
        removeRow(searchResult.row());
    }
    else
    {
        currentItem->setData(currentMusicCount-1, MusicCount);
    }
}

void KNMusicCategoryModel::onAlbumArtUpdate(const int &index)
{
    QString currentName=categoryName(index);
    if(currentName.isEmpty())
    {
        return;
    }
    QModelIndex searchResult=indexOf(currentName);
    if(!searchResult.isValid())
    {
        return;
    }
    QStandardItem *currentItem=itemFromIndex(searchResult);
    currentItem->setData(m_sourceModel->artworkKey(index), IconKey);
    currentItem->setData(itemIcon(index), Qt::DecorationRole);
}

void KNMusicCategoryModel::updateImage(const int &index)
{
    Q_UNUSED(index);
}

void KNMusicCategoryModel::setSourceModel(QAbstractItemModel *sourceModel)
{
    KNMusicLibraryModelBase *musicModel=qobject_cast<KNMusicLibraryModelBase *>(sourceModel);
    m_sourceModel=musicModel;
    connect(m_sourceModel, &KNMusicLibraryModel::musicAppend,
            this, &KNMusicCategoryModel::onMusicAdded);
    connect(m_sourceModel, &KNMusicLibraryModel::musicRecover,
            this, &KNMusicCategoryModel::onMusicRecover);
    connect(m_sourceModel, &KNMusicLibraryModel::musicAlbumArtUpdate,
            this, &KNMusicCategoryModel::onAlbumArtUpdate);
    connect(m_sourceModel, &KNMusicLibraryModel::requireUpdateImage,
            this, &KNMusicCategoryModel::updateAllImage);
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

void KNMusicCategoryModel::updateAllImage()
{
    for(int i=0, itemCount=rowCount(); i<itemCount; i++)
    {
        updateImage(i);
    }
}

void KNMusicCategoryModel::setNoCategoryText(const QString &noCategoryText)
{
    m_noCategoryText=noCategoryText;
}

QString KNMusicCategoryModel::filterString(const QModelIndex &index) const
{
    return data(index, Qt::UserRole).toString();
}

QIcon KNMusicCategoryModel::albumArt(const QModelIndex &index) const
{
    return data(index, Qt::DecorationRole).value<QIcon>();
}
