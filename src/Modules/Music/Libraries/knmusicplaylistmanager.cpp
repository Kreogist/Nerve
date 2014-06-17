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
#include "../Libraries/knmusicmodel.h"
#include "../Libraries/knmusicplaylistmodel.h"
#include "../Libraries/knmusicinfocollectormanager.h"
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
    m_playlistDataModel=new KNMusicPlaylistModel;
    m_playlistDataModel->moveToThread(&m_dataModelThread);
    m_infoCollectManager=new KNMusicInfoCollectorManager;
    m_infoCollectManager->moveToThread(&m_infoThread);
    m_playlistDataModel->setInfoCollectorManager(m_infoCollectManager);
    m_infoThread.start();
    m_dataModelThread.start();
}

KNMusicPlaylistManager::~KNMusicPlaylistManager()
{
    m_infoThread.quit();
    m_dataModelThread.quit();
    m_infoThread.wait();
    m_dataModelThread.wait();
    m_infoCollectManager->deleteLater();
    m_playlistDataModel->deleteLater();
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
        playlistItem->loadPlaylist();
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

void KNMusicPlaylistManager::setPlaylist(const int &index)
{
    KNMusicPlaylistItem *currentItem=
               static_cast<KNMusicPlaylistItem *>(m_playlistModel->item(index));
    QStringList fileList=currentItem->playlist();
    m_playlistDataModel->clear();
    m_playlistDataModel->resetHeader();
    for(int i=0; i<fileList.size(); i++)
    {
        QModelIndex fileSearch=m_musicModel->indexFromFilePath(fileList.at(i));
        if(fileSearch.isValid())
        {
            m_playlistDataModel->appendRow(m_musicModel->songRow(fileSearch.row()));
        }
        else
        {
            m_playlistDataModel->addRawFileItem(fileList.at(i));
        }
    }
}

void KNMusicPlaylistManager::saveAllChanged()
{
    for(int i=0, listSize=m_playlistModel->rowCount();
        i<listSize;
        i++)
    {
        //If changed.
        KNMusicPlaylistItem *currentItem=
                static_cast<KNMusicPlaylistItem *>(m_playlistModel->item(i, 0));
        currentItem->savePlayList();
    }
}

int KNMusicPlaylistManager::loopMode()
{
    return m_nowPlaying->loopMode();
}

void KNMusicPlaylistManager::setMusicBackend(KNLibBass *backend)
{
    m_infoCollectManager->setMusicBackend(backend);
}

void KNMusicPlaylistManager::setMusicModel(KNMusicModel *model)
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

QAbstractItemModel *KNMusicPlaylistManager::playlistModel()
{
    return m_playlistModel;
}

QAbstractItemModel *KNMusicPlaylistManager::playlistDataModel()
{
    return m_playlistDataModel;
}

void KNMusicPlaylistManager::retranslate()
{
    ;
}

void KNMusicPlaylistManager::retranslateAndSet()
{
    ;
}
