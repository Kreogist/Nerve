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
        return;
    }
    QList<QStandardItem *> searchResult=findItems(currentName);
    if(searchResult.size()==0)
    {
        currentItem=new KNMusicArtistItem(currentName);
        currentItem->setData(currentName, Qt::UserRole);
        currentItem->setIcon(itemIcon(index.row()));
        searchResult.append(currentItem);
        appendRow(currentItem);
    }
}

void KNMusicCategoryModel::setSourceModel(QAbstractItemModel *sourceModel)
{
    KNMusicModel *musicModel=qobject_cast<KNMusicModel *>(sourceModel);
    m_sourceModel=musicModel;
    connect(m_sourceModel, &KNMusicModel::musicAppend,
            this, &KNMusicCategoryModel::onMusicAdded);
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

