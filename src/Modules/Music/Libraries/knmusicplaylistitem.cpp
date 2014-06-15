#include <QFile>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonParseError>
#include <QJsonArray>

#include "knmusicplaylistitem.h"

KNMusicPlaylistItem::KNMusicPlaylistItem() :
    QStandardItem()
{
    setData(false, DataChanged);
}

KNMusicPlaylistItem::~KNMusicPlaylistItem()
{
    ;
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

void KNMusicPlaylistItem::setPlaylist(const QStringList &data)
{
    setData(data, SongsPath);
    setData(true, DataChanged);
}

QStringList KNMusicPlaylistItem::playlist() const
{
    return data(SongsPath).toStringList();
}

bool KNMusicPlaylistItem::loadPlaylist()
{
    //Open the playlist file.
    QFile playlistFile(data(FilePath).toString());
    if(!playlistFile.exists() ||
            !playlistFile.open(QIODevice::ReadOnly))
    {
        resetPlaylist(playlistFile.fileName());
        return false;
    }
    //Read the playlist from file.
    QByteArray readCache=playlistFile.readAll();
    playlistFile.close();
    //Parse document.
    QJsonDocument playlistContent=QJsonDocument::fromJson(readCache);
    if(playlistContent.isNull())
    {
        resetPlaylist(playlistFile.fileName());
        return false;
    }
    QJsonObject playlistObject=playlistContent.object();
    //Set title.
    setText(playlistObject["Name"].toString());
    //Append data to list.
    QJsonArray songList=playlistObject["Songs"].toArray();
    //Process Data
    QStringList songPathList;
    for(int i=0, songCount=songList.size();
        i<songCount;
        i++)
    {
        songPathList.append(songList.at(i).toString());
    }
    setData(songPathList, SongsPath);
    setData(false, DataChanged);
    return true;
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
        QJsonArray songList=QJsonArray::fromStringList(data(SongsPath).toStringList());
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

void KNMusicPlaylistItem::resetPlaylist(const QString &fileName)
{
    setName(fileName.isEmpty()?data(FilePath).toString():fileName);
    setData(QStringList(), SongsPath);
    setData(false, DataChanged);;
}
