#include <QDebug>

#include "knmusicplaylistmodel.h"

#include "knmusicplaylistitem.h"

KNMusicPlaylistItem::KNMusicPlaylistItem() :
    QStandardItem()
{
    setData(false, DataChanged);
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
    setData(true, DataChanged);
}

bool KNMusicPlaylistItem::changed()
{
    return data(DataChanged).toBool();
}

void KNMusicPlaylistItem::setChanged(const bool &changed)
{
    setData(changed, DataChanged);
}

QString KNMusicPlaylistItem::filePath() const
{
    return data(PlaylistPath).toString();
}

void KNMusicPlaylistItem::setFilePath(const QString &path)
{
    setData(path, PlaylistPath);
    setData(true, DataChanged);
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
    setData(false, DataChanged);
}

QStringList KNMusicPlaylistItem::songPaths() const
{
    return m_songPaths;
}

void KNMusicPlaylistItem::appendSongRow(const QList<QStandardItem *> &rowData)
{
    m_playlistModel->appendRow(rowData);
}

void KNMusicPlaylistItem::appendSongItem(QStringList textList,
                                         KNMusicGlobal::MusicDetailsInfo currentDetails)
{
    m_playlistModel->appendMusicItem(textList, currentDetails);
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

