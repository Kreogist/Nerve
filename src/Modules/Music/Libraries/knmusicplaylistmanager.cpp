#include <QFile>
#include <QDir>
#include <QDateTime>
#include <QSortFilterProxyModel>
#include <QFileInfo>

#include <QDebug>

#include "knmusicnowplaying.h"
#include "../../knglobal.h"

#include "knmusicplaylistmanager.h"

KNMusicPlaylistManager::KNMusicPlaylistManager(QObject *parent) :
    QObject(parent)
{
    //Get the latest translation.
    retranslate();
    //Initial global settings.
    m_global=KNGlobal::instance();
    m_musicGlobal=KNMusicGlobal::instance();
    //Initial pathes.
    m_playlistPath=m_global->databaseFolder()+"/Playlists/";
    QDir playlistDir(m_playlistPath);
    if(!playlistDir.exists())
    {
        playlistDir.mkpath(m_playlistPath);
    }
    m_configureFile=new QFile(m_playlistPath+"Playlists.db", this);
    //Initial category filter model.
    m_nowPlaying=new KNMusicNowPlaying(this);
}

KNMusicPlaylistManager::~KNMusicPlaylistManager()
{
    saveAllChanged();
    savePlayLists();
}

void KNMusicPlaylistManager::loadPlayLists()
{
    //Open configure file.
    if(!m_configureFile->open(QIODevice::ReadOnly))
    {
        return;
    }
    QByteArray readCache=m_configureFile->readAll();
    m_configureFile->close();
    //Parse configure data.
    m_configureContent=QJsonDocument::fromJson(readCache, &m_error);
    if(m_configureContent.isNull())
    {
        return;
    }
    m_configure=m_configureContent.object();
    m_playlists=m_configure["Playlists"].toArray();
}

void KNMusicPlaylistManager::savePlayLists()
{
    m_configure["Playlists"]=m_playlists;
    m_configureContent.setObject(m_configure);
    m_configureFile->open(QIODevice::WriteOnly);
    m_configureFile->write(m_configureContent.toJson());
    m_configureFile->close();
}

bool KNMusicPlaylistManager::loadPlayList(const QString &filePath)
{
    //Open the playlist file.
    QFile playlistFile(filePath);
    if(!playlistFile.exists() ||
            !playlistFile.open(QIODevice::ReadOnly))
    {
        return false;
    }
    //Read the playlist from file.
    QByteArray readCache=playlistFile.readAll();
    playlistFile.close();
    //Parse document.
    QJsonDocument playlistContent=QJsonDocument::fromJson(readCache);
    if(playlistContent.isNull())
    {
        return false;
    }
    QJsonObject playlistObject=playlistContent.object();
    //Append data to list.
    QJsonArray songList=playlistObject["Songs"].toArray();
    //Process Data
    return true;
}

void KNMusicPlaylistManager::setLoopMode(const int &index)
{
    m_nowPlaying->setLoopMode(index);
}

void KNMusicPlaylistManager::createPlaylist(const QString &title)
{
    //Prepare the playlist data.
    //Prepare the file name.
    QString baseName=QDateTime::currentDateTime().toString("yyyyMMddhhmmsszzz"),
            playlistPath=m_playlistPath+
                         baseName+".json";
    QFileInfo playlistFile(playlistPath);
    int count=1;
    while(playlistFile.exists())
    {
        playlistPath=m_playlistPath+
                     baseName+" "+
                     QString::number(count++)+".json";
        playlistFile.setFile(playlistPath);
    }
    //Add to playlist list.
    ;
    //Save to file.
    ;
}

void KNMusicPlaylistManager::importPlaylist(QStringList filePaths)
{
    for(int i=0; i<filePaths.size(); i++)
    {
        QString currentPath=filePaths.at(i);
        //Process.
    }
}

void KNMusicPlaylistManager::removePlaylist(const int &index)
{
//    QString removedPath;
//    if(removedPath.contains(m_playlistPath))
//    {
//        //It's a path in playlist menu, delete it.
//        QFile currentFile(removedPath);
//        currentFile.remove();
//    }
}

void KNMusicPlaylistManager::saveAllChanged()
{
//    for(int i=0, listSize=m_playlistList.size();
//        i<listSize;
//        i++)
//    {
//        //If changed.
//            savePlayList(i);
//    }
}

void KNMusicPlaylistManager::savePlayList(const int &index)
{
//    KNPlayList saveList=m_playlistList.at(index);
//    QFile playlistFile(m_playlists.at(index).toString());
//    if(playlistFile.open(QIODevice::WriteOnly))
//    {
//        QJsonObject playlistObject;
//        playlistObject["Name"]=m_playlistNameList.at(index);
//        QJsonArray songList;
//        for(int i=0, songCount=saveList.songs.size();
//            i<songCount;
//            i++)
//        {
//            songList.append(saveList.songs.at(i));
//        }
//        playlistObject["Songs"]=songList;
//        QJsonDocument playlistContent;
//        playlistContent.setObject(playlistObject);
//        playlistFile.write(playlistContent.toJson());
//        playlistFile.close();
//        saveList.changed=false;
//        m_playlistList.replace(index, saveList);
//    }
}

int KNMusicPlaylistManager::loopMode()
{
    return m_nowPlaying->loopMode();
}

void KNMusicPlaylistManager::setMusicModel(KNMusicModel *model)
{
    m_nowPlaying->setMusicModel(model);
}

void KNMusicPlaylistManager::setProxyModel(QSortFilterProxyModel *model)
{
    m_nowPlaying->setProxyModel(model);
}

void KNMusicPlaylistManager::setCurrentPlaying(const QString &string)
{
    m_nowPlaying->setCurrentPlaying(string);
}

void KNMusicPlaylistManager::addToCurrentList(const QString &index)
{
    ;
}

QString KNMusicPlaylistManager::nextPlayingSong()
{
    return m_nowPlaying->nextPlayingSong();
}

QString KNMusicPlaylistManager::nextSong()
{
    return m_nowPlaying->nextSong();
}

QString KNMusicPlaylistManager::prevSong()
{
    return m_nowPlaying->prevSong();
}

void KNMusicPlaylistManager::retranslate()
{
    ;
}

void KNMusicPlaylistManager::retranslateAndSet()
{
    ;
}
