#include <QDebug>

#include "knmusicplaylistmodel.h"

#include "knmusicplaylistitem.h"

KNMusicPlaylistItem::KNMusicPlaylistItem() :
    QStandardItem()
{
    m_dataChanged=false;
    setData(false, ModelBuild);
    m_playlistModel=new KNMusicPlaylistModel;
}

KNMusicPlaylistItem::~KNMusicPlaylistItem()
{
    m_playlistModel->deleteLater();
}

void KNMusicPlaylistItem::setName(const QString &name)
{
    setText(name);
    setChanged(true);
}

bool KNMusicPlaylistItem::changed()
{
    return m_dataChanged;
}

void KNMusicPlaylistItem::setChanged(const bool &changed)
{
    qDebug()<<this->index()<<changed;
    m_dataChanged=changed;
}

QString KNMusicPlaylistItem::filePath() const
{
    return data(PlaylistPath).toString();
}

void KNMusicPlaylistItem::setFilePath(const QString &path)
{
    setData(path, PlaylistPath);
    setChanged(true);
}

bool KNMusicPlaylistItem::modelBuild() const
{
    return data(ModelBuild).toBool();
}

QModelIndex KNMusicPlaylistItem::firstIndexFromMusicPath(const QString &filePath)
{
    return m_playlistModel->indexFromFilePath(filePath);
}

QList<QStandardItem *> KNMusicPlaylistItem::songRow(const int &row) const
{
    return m_playlistModel->songRow(row);
}

QStandardItemModel *KNMusicPlaylistItem::playlistModel()
{
    return m_playlistModel;
}

void KNMusicPlaylistItem::clearSongPaths()
{
    m_songPaths.clear();
    setData(true, ModelBuild);
}

void KNMusicPlaylistItem::resetPlaylist(const QString &fileName)
{
    setName(fileName.isEmpty()?data(PlaylistPath).toString():fileName);
    setChanged(false);
}

QStringList KNMusicPlaylistItem::songPaths() const
{
    return m_songPaths;
}

void KNMusicPlaylistItem::appendSongRow(const QList<QStandardItem *> &rowData)
{
    m_playlistModel->appendRow(rowData);
    setChanged(true);
}

void KNMusicPlaylistItem::appendSongItem(QStringList textList,
                                         KNMusicGlobal::MusicDetailsInfo currentDetails)
{
    m_playlistModel->appendMusicItem(textList, currentDetails);
    setChanged(true);
}

int KNMusicPlaylistItem::songCount() const
{
    return m_playlistModel->rowCount();
}

QString KNMusicPlaylistItem::songPath(const int &songIndex) const
{
    return m_playlistModel->filePathFromIndex(songIndex);
}

void KNMusicPlaylistItem::setSongPaths(const QStringList &songPaths)
{
    m_songPaths=songPaths;
}

