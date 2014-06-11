#include <QFile>
#include <QDir>
#include <QDateTime>
#include <QSortFilterProxyModel>
#include <QFileInfo>

#include <QDebug>

#include "../Libraries/knmusicmodel.h"
#include "../Libraries/knmusiccategorydetailmodel.h"
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
    m_categoryProxyModel=new KNMusicCategoryDetailModel(this);
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
    //Load all playlists.
    for(int i=0; i<m_playlists.size(); i++)
    {
        if(!loadPlayList(m_playlists.at(i).toString()))
        {
            m_playlists.removeAt(i);
            i--;
        }
    }
    //Emit data updated signal.
    emit playlistListUpdated();
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
    KNPlayList playlist;
    playlist.path=filePath;
    QJsonArray songList=playlistObject["Songs"].toArray();
    for(int i=0, songCount=songList.size(); i<songCount; i++)
    {
        playlist.songs.append(songList.at(i).toString());
    }
    //Add playlist to list.
    m_playlistNameList.append(playlistObject["Name"].toString());
    m_playlistList.append(playlist);
    return true;
}

QStringList KNMusicPlaylistManager::playlistNameList() const
{
    return m_playlistNameList;
}

void KNMusicPlaylistManager::setLoopMode(const int &index)
{
    m_loopMode=index;
}

void KNMusicPlaylistManager::addPlaylist(const QString &title)
{
    //Prepare the playlist data.
    KNPlayList playlist;
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
    m_playlists.append(playlistPath);
    playlist.path=playlistPath;
    //Add to playlist list.
    m_playlistNameList.append(title);
    m_playlistList.append(playlist);
    //Save to file.
    savePlayList(m_playlistList.size()-1);
}

void KNMusicPlaylistManager::removePlaylist(const int &index)
{
    m_playlistNameList.removeAt(index);
    m_playlistList.removeAt(index);
    m_playlists.removeAt(index);
}

QString KNMusicPlaylistManager::currentIndexPath()
{
    return m_currentPath;
}

void KNMusicPlaylistManager::saveAllChanged()
{
    for(int i=0, listSize=m_playlistList.size();
        i<listSize;
        i++)
    {
        if(m_playlistList.at(i).changed)
        {
            savePlayList(i);
        }
    }
}

void KNMusicPlaylistManager::savePlayList(const int &index)
{
    KNPlayList saveList=m_playlistList.at(index);
    QFile playlistFile(saveList.path);
    if(playlistFile.open(QIODevice::WriteOnly))
    {
        QJsonObject playlistObject;
        playlistObject["Name"]=m_playlistNameList.at(index);
        QJsonArray songList;
        for(int i=0, songCount=saveList.songs.size();
            i<songCount;
            i++)
        {
            songList.append(saveList.songs.at(i));
        }
        playlistObject["Songs"]=songList;
        QJsonDocument playlistContent;
        playlistContent.setObject(playlistObject);
        playlistFile.write(playlistContent.toJson());
        playlistFile.close();
        saveList.changed=false;
        m_playlistList.replace(index, saveList);
    }
}

int KNMusicPlaylistManager::loopMode()
{
    return m_loopMode;
}

void KNMusicPlaylistManager::setMusicModel(KNMusicModel *model)
{
    m_musicModel=model;
    m_categoryProxyModel->setSourceModel(m_musicModel);
}

void KNMusicPlaylistManager::setProxyModel(QSortFilterProxyModel *model)
{
    m_usingProxy=true;
    if(model->filterRegExp().pattern().isEmpty())
    {
        m_proxyModel=model;
        emit requireUpdatePlaylistModel(m_proxyModel);
        return;
    }
    m_categoryProxyModel->setFilterRegExp(model->filterRegExp());
    m_categoryProxyModel->setSortCaseSensitivity(model->sortCaseSensitivity());
    m_categoryProxyModel->setSortRole(model->sortRole());
    m_categoryProxyModel->setFilterKeyColumn(model->filterKeyColumn());
    m_categoryProxyModel->sort(model->sortColumn(), model->sortOrder());
    m_proxyModel=m_categoryProxyModel;
    emit requireUpdatePlaylistModel(m_proxyModel);
}

void KNMusicPlaylistManager::playNow(const QString &string)
{
    //If using a proxy, search in the proxy model.
    if(m_usingProxy)
    {
        m_currentPath=string;
        return;
    }
    //Or else, search in the playlist.
    if(m_currentList->songs.indexOf(string)==-1)
    {
        addToCurrentList(string);
    }
    m_currentPath=string;
}

void KNMusicPlaylistManager::addToCurrentList(const QString &index)
{
    m_currentList->songs.append(index);
}

QString KNMusicPlaylistManager::nextSong()
{
    /*! FIXME:
      Judge whether there's songs wait for playing next*/
    ;
    if(m_loopMode==KNMusicGlobal::RepeatSong)
    {
        return m_currentPath;
    }
    return nextListSong();
}

QString KNMusicPlaylistManager::nextListSong()
{
    //If we are using proxy mode, search in the proxy model.
    if(m_usingProxy)
    {
        //If current path is empty, set to the initial position.
        if(m_currentPath.isEmpty())
        {
            m_currentPath=
                    m_musicModel->filePathFromIndex(
                        m_proxyModel->mapToSource(m_proxyModel->index(0,0)));
            return m_currentPath;
        }
        //Translate to index.
        int nowPlayingIndex=m_proxyModel->mapFromSource(
                    m_musicModel->indexFromFilePath(m_currentPath)).row();
        //Switch to the next according to the loop mode.
        switch(m_loopMode)
        {
        case KNMusicGlobal::RepeatAll:
            nowPlayingIndex=nowPlayingIndex==m_proxyModel->rowCount()-1?
                        0:nowPlayingIndex+1;
            break;
        case KNMusicGlobal::RepeatSong:
        case KNMusicGlobal::NoRepeat:
            if(nowPlayingIndex==m_proxyModel->rowCount()-1)
            {
                m_currentPath.clear();
                return QString();
            }
            nowPlayingIndex++;
            break;
        }
        //Translate back to file path.
        m_currentPath=m_musicModel->filePathFromIndex(
                    m_proxyModel->mapToSource(m_proxyModel->index(nowPlayingIndex,0)));
        return m_currentPath;
    }
    //Means using the now playing list.
    //Check whether the list is empty first.
    if(m_currentList->songs.isEmpty())
    {
        return QString();
    }
    //If it's not empty but the current index is -1(invaild),
    //Set to the first.
    if(m_currentPath.isEmpty())
    {
        m_currentPath=m_currentList->songs.first();
        return m_currentPath;
    }
    int nowPlayingIndex=m_currentList->songs.indexOf(m_currentPath);
    if(nowPlayingIndex==-1)
    {
        nowPlayingIndex=m_currentList->lastPlayingIndex;
    }
    switch(m_loopMode)
    {
    //RepeatSong: Repeat the single song.
    case KNMusicGlobal::RepeatAll:
        nowPlayingIndex=nowPlayingIndex>m_currentList->songs.size()-1?
                    0:nowPlayingIndex+1;
        break;
    case KNMusicGlobal::RepeatSong:
    case KNMusicGlobal::NoRepeat:
        if(nowPlayingIndex>m_currentList->songs.size()-1)
        {
            m_currentPath.clear();
            return QString();
        }
        nowPlayingIndex++;
        break;
    }
    m_currentList->lastPlayingIndex=nowPlayingIndex;
    m_currentPath=m_currentList->songs.at(nowPlayingIndex);
    return m_currentPath;
}

QString KNMusicPlaylistManager::prevListSong()
{
    //If we are using proxy mode, search in the proxy model.
    if(m_usingProxy)
    {
        //If current path is empty, set to the initial position.
        if(m_currentPath.isEmpty())
        {
            m_currentPath=
                    m_musicModel->filePathFromIndex(
                        m_proxyModel->mapToSource(m_proxyModel->index(m_proxyModel->rowCount()-1,
                                                                      0)));
            return m_currentPath;
        }
        //Translate to index.
        int nowPlayingIndex=m_proxyModel->mapFromSource(
                    m_musicModel->indexFromFilePath(m_currentPath)).row();
        //Switch to the next according to the loop mode.
        switch(m_loopMode)
        {
        case KNMusicGlobal::RepeatAll:
            nowPlayingIndex=nowPlayingIndex==0?
                        m_proxyModel->rowCount()-1:nowPlayingIndex-1;
            break;
        case KNMusicGlobal::RepeatSong:
        case KNMusicGlobal::NoRepeat:
            if(nowPlayingIndex==0)
            {
                m_currentPath.clear();
                return QString();
            }
            nowPlayingIndex--;
            break;
        }
        //Translate back to file path.
        m_currentPath=m_musicModel->filePathFromIndex(
                    m_proxyModel->mapToSource(m_proxyModel->index(nowPlayingIndex,0)));
        return m_currentPath;
    }
    //Means using the now playing list.
    //Check whether the list is empty first.
    if(m_currentList->songs.isEmpty())
    {
        return QString();
    }
    //If it's not empty but the current index is -1(invaild),
    //Set to the first.
    if(m_currentPath.isEmpty())
    {
        m_currentPath=m_currentList->songs.last();
        return m_currentPath;
    }
    int nowPlayingIndex=m_currentList->songs.indexOf(m_currentPath);
    if(nowPlayingIndex==-1)
    {
        nowPlayingIndex=m_currentList->lastPlayingIndex;
    }
    switch(m_loopMode)
    {
    //RepeatSong: Repeat the single song.
    case KNMusicGlobal::RepeatAll:
        nowPlayingIndex=nowPlayingIndex<1?
                    m_currentList->songs.size()-1:nowPlayingIndex-1;
        break;
    case KNMusicGlobal::RepeatSong:
    case KNMusicGlobal::NoRepeat:
        if(nowPlayingIndex<1)
        {
            m_currentPath.clear();
            return QString();
        }
        nowPlayingIndex--;
        break;
    }
    m_currentList->lastPlayingIndex=nowPlayingIndex;
    m_currentPath=m_currentList->songs.at(nowPlayingIndex);
    return m_currentPath;
}

void KNMusicPlaylistManager::retranslate()
{
    ;
}

void KNMusicPlaylistManager::retranslateAndSet()
{
    ;
}
