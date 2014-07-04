#include <QMimeData>

#include <QDebug>

#include "knmusicplaylistlistmodel.h"

KNMusicPlaylistListModel::KNMusicPlaylistListModel(QObject *parent) :
    QStandardItemModel(parent)
{
    ;
}

Qt::DropActions KNMusicPlaylistListModel::supportedDropActions() const
{
    return Qt::CopyAction | Qt::MoveAction;
}

Qt::ItemFlags KNMusicPlaylistListModel::flags(const QModelIndex &index) const
{
    if(index.isValid())
    {
        return Qt::ItemIsDropEnabled | Qt::ItemIsDragEnabled | QStandardItemModel::flags(index);
    }
    return Qt::ItemIsDropEnabled;
}

QStringList KNMusicPlaylistListModel::mimeTypes() const
{
    QStringList types=QStandardItemModel::mimeTypes();
    types<<"text/uri-list";
    return types;
}

bool KNMusicPlaylistListModel::dropMimeData(const QMimeData *data,
                                            Qt::DropAction action,
                                            int row,
                                            int column,
                                            const QModelIndex &parent)
{
    qDebug()<<data->formats();
    if(data->hasUrls())
    {
        ;
    }
    return true;
}
