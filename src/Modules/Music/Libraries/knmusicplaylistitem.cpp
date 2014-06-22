#include <QFile>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonParseError>
#include <QJsonArray>

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

bool KNMusicPlaylistItem::removedFile()
{
    //Open the playlist file.
    QFile playlistFile(data(FilePath).toString());
    if(playlistFile.exists())
    {
        return playlistFile.remove();
    }
    return true;
}

QString KNMusicPlaylistItem::filePath() const
{
    return data(FilePath).toString();
}

void KNMusicPlaylistItem::setFilePath(const QString &path)
{
    setData(path, FilePath);
    setData(true, DataChanged);
}

bool KNMusicPlaylistItem::savePlayList()
{
    if(!data(DataChanged).toBool())
    {
        //Nothing to write.
        return true;
    }
    return forceSavePlaylist();
}

bool KNMusicPlaylistItem::forceSavePlaylist()
{
    //Open the playlist file.
    QFile playlistFile(data(FilePath).toString());
    if(playlistFile.open(QIODevice::WriteOnly))
    {
        //Prepare the Json Data.
        QJsonObject playlistObject;
        playlistObject["Name"]=text();
        QJsonArray songList;
        //!FIXME: songList should contains all the song file path.
        playlistObject["Songs"]=songList;
        QJsonDocument playlistContent;
        playlistContent.setObject(playlistObject);
        //Rewrite data.
        playlistFile.write(playlistContent.toJson());
        playlistFile.close();
        //Clear the data changed flag.
        setData(false, DataChanged);
        return true;
    }
    return false;
}

bool KNMusicPlaylistItem::modelBuild() const
{
    return data(ModelBuild).toBool();
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
    setName(fileName.isEmpty()?data(FilePath).toString():fileName);
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

void KNMusicPlaylistItem::setSongPaths(const QStringList &songPaths)
{
    m_songPaths=songPaths;
}

