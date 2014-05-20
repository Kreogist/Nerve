#include <QDebug>

#include "../knmusicglobal.h"

#include "knmusiccategorymodel.h"

KNMusicCategoryModel::KNMusicCategoryModel(QObject *parent) :
    QAbstractListModel(parent)
{
    retranslate();

    m_noAlbumArtIcon=QIcon(QPixmap::fromImage(KNMusicGlobal::instance()->noAlbumImage()));

    resetModel();
}

int KNMusicCategoryModel::rowCount(const QModelIndex &parent) const
{
    if(parent.isValid())
    {
        return 1;
    }
    return m_detailList.size();
}

int KNMusicCategoryModel::columnCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return 1;
}

QVariant KNMusicCategoryModel::data(const QModelIndex &index, int role) const
{
    if(!index.isValid())
    {
        return QVariant();
    }
    if(index.row()<m_textList.size())
    {
        switch(role)
        {
        case Qt::DisplayRole:
            if(index.row()==0)
            {
                return m_noCategoryText;
            }
            return m_textList.at(index.row());
        case Qt::EditRole:
            return m_textList.at(index.row());
        case Qt::DecorationRole:
            return m_detailList.at(index.row()).decoration;
        case Qt::ToolTipRole:
        case Qt::StatusTipRole:
        case Qt::WhatsThisRole:
            return QString();
        case Qt::SizeHintRole:
            return QSize(35, 40);
        }
    }
    return QVariant();
}

QVariant KNMusicCategoryModel::headerData(int section,
                                          Qt::Orientation orientation,
                                          int role) const
{
    Q_UNUSED(section);
    Q_UNUSED(orientation);
    Q_UNUSED(role);
    return QVariant();
}

bool KNMusicCategoryModel::setData(const QModelIndex &index,
                                   const QVariant &value,
                                   int role)
{
    if(index.isValid())
    {
        return false;
    }
    MusicCategoryItem currentItem;
    switch(role)
    {
    case Qt::DisplayRole:
    case Qt::EditRole:
        m_textList.replace(index.row(), value.toString());
        emit dataChanged(index, index, QVector<int>(1, role));
        return true;
    case Qt::DecorationRole:
        currentItem=m_detailList.at(index.row());
        currentItem.decoration=value.value<QIcon>();
        m_detailList.replace(index.row(), currentItem);
        emit dataChanged(index, index, QVector<int>(1, Qt::DecorationRole));
        return true;
    case Qt::ToolTipRole:
    case Qt::StatusTipRole:
    case Qt::WhatsThisRole:
        return false;
    case Qt::SizeHintRole:
        return false;
    }
    return false;
}

QModelIndex KNMusicCategoryModel::indexOf(const QString &text) const
{
    return index(m_textList.indexOf(text));
}

bool KNMusicCategoryModel::removeRows(int row, int count, const QModelIndex &parent)
{
    Q_UNUSED(parent);
    while(count-- && row<m_textList.size())
    {
        m_textList.removeAt(row);
        m_detailList.removeAt(row);
        onActionRemoveRow(row);
    }
    return true;
}

void KNMusicCategoryModel::resetModel()
{
    //Clear the QList
    m_textList.clear();
    m_detailList.clear();

    //Initial the first no category text item.
    MusicCategoryItem currentCategory;
    currentCategory.decoration=m_noAlbumArtIcon;
    currentCategory.songCount=0;
    insertCategoryRow("", currentCategory);
}

QString KNMusicCategoryModel::filterText(const QModelIndex &index) const
{
    return m_textList.at(index.row());
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
    if(currentName.isEmpty())
    {
        m_noCategoryItemCount++;
        emit requireShowFirstItem();
        return;
    }
    int searchResult=m_textList.indexOf(currentName);
    MusicCategoryItem currentItem;
    if(searchResult==-1)
    {
        currentItem.songCount=1;
        insertCategoryRow(currentName, currentItem);
    }
    else
    {
        currentItem=m_detailList.at(searchResult);
        currentItem.songCount++;
        m_detailList.replace(searchResult,
                             currentItem);
    }
}

void KNMusicCategoryModel::onMusicRecover(const QModelIndex &index)
{
    QString currentName=categoryName(index.row());
    if(currentName.isEmpty())
    {
        m_noCategoryItemCount++;
        emit requireShowFirstItem();
        return;
    }
    int searchResult=m_textList.indexOf(currentName);
    MusicCategoryItem currentItem;
    if(searchResult==-1)
    {
        currentItem.songCount=1;
        currentItem.iconKey=m_sourceModel->itemArtworkKey(index.row());
        insertCategoryRow(currentName, currentItem);
    }
    else
    {
        currentItem=m_detailList.at(searchResult);
        currentItem.songCount++;
        m_detailList.replace(searchResult,
                             currentItem);
    }
}

void KNMusicCategoryModel::onMusicRemoved(const QModelIndex &index)
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
    MusicCategoryItem currentItem=m_detailList.at(searchResult);
    if(currentItem.songCount==1)
    {
        removeRow(searchResult);
    }
    else
    {
        currentItem.songCount--;
        m_detailList.replace(searchResult, currentItem);
    }
}

void KNMusicCategoryModel::onAlbumArtUpdate(const int &index)
{
    QString currentName=categoryName(index);
    if(currentName.isEmpty())
    {
        return;
    }
    int searchResult=m_textList.indexOf(currentName);
    if(searchResult==-1)
    {
        return;
    }
    MusicCategoryItem currentItem=m_detailList.at(searchResult);
    currentItem.iconKey=m_sourceModel->itemArtworkKey(index);
    currentItem.decoration=itemIcon(index);
    m_detailList.replace(searchResult, currentItem);
}

void KNMusicCategoryModel::updateImage(const int &index)
{
    Q_UNUSED(index);
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
    connect(m_sourceModel, &KNMusicModel::requireUpdateImage,
            this, &KNMusicCategoryModel::updateAllImage);
}

KNMusicCategoryModel::MusicCategoryItem KNMusicCategoryModel::item(const int &index) const
{
    return m_detailList.at(index);
}

void KNMusicCategoryModel::insertCategoryRow(const QString &text,
                                          const KNMusicCategoryModel::MusicCategoryItem &details)
{
    emit beginInsertRows(QModelIndex(), m_textList.size(), m_textList.size()+1);
    m_textList.append(text);
    m_detailList.append(details);
    emit endInsertRows();
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

void KNMusicCategoryModel::onActionRemoveRow(const int &index)
{
    Q_UNUSED(index);
}

void KNMusicCategoryModel::updateAllImage()
{
    for(int i=0, itemCount=m_textList.size(); i<itemCount; i++)
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
