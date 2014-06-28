#include <QFile>
#include <QDir>
#include <QStandardItemModel>
#include <QDateTime>
#include <QSortFilterProxyModel>
#include <QDataStream>
#include <QFileInfo>

#include <QDebug>

#include "knmusicplaylistitem.h"
#include "knmusicnowplaying.h"
#include "../Libraries/knmusiclibrarymodel.h"
#include "../Libraries/knmusicinfocollector.h"
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
    connect(m_nowPlaying, &KNMusicNowPlaying::requireUpdatePlaylistModel,
            this, &KNMusicPlaylistManager::requireUpdatePlaylistModel);
    //Initial playlist model.
    m_playlistModel=new QStandardItemModel(this);
    //Initial info collector.
    m_infoCollector=new KNMusicInfoCollector(this);
    m_infoCollector->setSignalMode(false);
//    m_infoCollector->moveToThread(&m_infoThread);
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
    //Parse configure data.
    for(int i=0, playlistCount=m_playlists.size();
        i<playlistCount;
        i++)
    {
        KNMusicPlaylistItem *playlistItem=new KNMusicPlaylistItem;
        playlistItem->setFilePath(m_playlists.at(i).toString());
        loadPlaylist(playlistItem);
        m_playlistModel->appendRow(playlistItem);
    }
}

void KNMusicPlaylistManager::savePlayLists()
{
    m_configure["Playlists"]=m_playlists;
    m_configureContent.setObject(m_configure);
    m_configureFile->open(QIODevice::WriteOnly);
    m_configureFile->write(m_configureContent.toJson());
    m_configureFile->close();
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
    KNMusicPlaylistItem *removedItem=
            static_cast<KNMusicPlaylistItem *>(m_playlistModel->item(index, 0));
    QString removedPath=removedItem->filePath();
    if(removedPath.contains(m_playlistPath))
    {
        //It's a path in playlist menu, delete it.
        if(!removedItem->removedFile())
        {
            //Cannot remove the file.
            qDebug()<<"Cannot remove file!";
            return;
        }
    }
    m_playlistModel->removeRow(index);
}

QString KNMusicPlaylistManager::setModelPlaylist(const int &index)
{
    KNMusicPlaylistItem *currentItem=
               static_cast<KNMusicPlaylistItem *>(m_playlistModel->item(index));
    ;
    return currentItem->filePath();
}

void KNMusicPlaylistManager::setPlaylist(const QString &filePath)
{
    QModelIndexList fileCheck=m_playlistModel->match(m_playlistModel->index(0,0),
                                                     KNMusicPlaylistItem::FilePath,
                                                     filePath);
    if(fileCheck.isEmpty())
    {
        return;
    }
    KNMusicPlaylistItem *currentItem=
               static_cast<KNMusicPlaylistItem *>(m_playlistModel->item(fileCheck.first().row()));
    m_nowPlaying->setPlaylist(currentItem->playlistModel());
}

bool KNMusicPlaylistManager::loadPlaylist(KNMusicPlaylistItem *item)
{
    //Open the playlist file.
    QFile playlistFile(item->filePath());
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
    //Set title.
    item->setText(playlistObject["Name"].toString());
    //Append data to list.
    QJsonArray songDataList=playlistObject["Songs"].toArray();
    QStringList songList;
    //Process Data
    for(int i=0, songCount=songDataList.size();
        i<songCount;
        i++)
    {
        songList.append(songDataList.at(i).toString());
    }
    item->setSongPaths(songList);
    return true;
}

QAbstractItemModel *KNMusicPlaylistManager::buildPlaylist(KNMusicPlaylistItem *item)
{
    QStringList parseList=item->songPaths();
    for(int i=0, songCount=parseList.size();
        i<songCount;
        i++)
    {
        QString filePath=parseList.at(i);
        QModelIndex databaseIndex=m_musicModel->indexFromFilePath(filePath);
        if(databaseIndex.isValid())
        {
            item->appendSongRow(m_musicModel->songRow(databaseIndex.row()));
        }
        else
        {
            //Check the song has been load before. If it does, load it.
            QModelIndex exsistCheck=item->firstIndexFromMusicPath(filePath);
            if(exsistCheck.isValid())
            {
                item->appendSongRow(item->songRow(exsistCheck.row()));
                continue;
            }
            m_infoCollector->analysis(filePath);
            if(m_infoCollector->currentMusicValue().isEmpty())
            {
                //!FIXME: Here should display when the file is not exsist.
                continue;
            }
            item->appendSongItem(m_infoCollector->currentMusicValue(),
                                 m_infoCollector->currentMusicDatas());
        }
    }
    item->clearSongPaths();
    return item->playlistModel();
}

void KNMusicPlaylistManager::saveAllChanged()
{
//    for(int i=0, listSize=m_playlistModel->rowCount();
//        i<listSize;
//        i++)
//    {
//        //If changed.
//        KNMusicPlaylistItem *currentItem=
//                static_cast<KNMusicPlaylistItem *>(m_playlistModel->item(i, 0));
//        currentItem->savePlayList();
//    }
}

int KNMusicPlaylistManager::loopMode()
{
    return m_nowPlaying->loopMode();
}

void KNMusicPlaylistManager::setMusicBackend(KNMusicBackend *backend)
{
    m_infoCollector->setMusicBackend(backend);
}

void KNMusicPlaylistManager::setMusicModel(KNMusicLibraryModel *model)
{
    m_musicModel=model;
    m_nowPlaying->setMusicModel(m_musicModel);
}

void KNMusicPlaylistManager::setProxyModel(QSortFilterProxyModel *model)
{
    m_nowPlaying->setProxyModel(model);
}

void KNMusicPlaylistManager::setCurrentPlaying(const QString &string)
{
    m_nowPlaying->setCurrentPlaying(string);
}

void KNMusicPlaylistManager::setCurrentPlaylistPlaying(const QModelIndex &index)
{
    m_nowPlaying->setCurrentPlaying(index);
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

QString KNMusicPlaylistManager::playlistName(const QModelIndex &index) const
{
    return m_playlistModel->data(index, Qt::DisplayRole).toString();
}

QString KNMusicPlaylistManager::filePathFromIndex(const QModelIndex &index) const
{
    return m_nowPlaying->filePathFromIndex(index);
}

QAbstractItemModel *KNMusicPlaylistManager::playlistModel()
{
    return m_playlistModel;
}

QAbstractItemModel *KNMusicPlaylistManager::playlistDataModel(const QModelIndex &index)
{
    KNMusicPlaylistItem *currentItem=
               static_cast<KNMusicPlaylistItem *>(m_playlistModel->itemFromIndex(index));
    if(currentItem->modelBuild())
    {
        return currentItem->playlistModel();
    }
    return buildPlaylist(currentItem);
}

void KNMusicPlaylistManager::retranslate()
{
    ;
}

void KNMusicPlaylistManager::retranslateAndSet()
{
    ;
}
