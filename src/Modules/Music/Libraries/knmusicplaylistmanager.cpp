#include <QFile>
#include <QDir>
#include <QDateTime>
#include <QSortFilterProxyModel>
#include <QDataStream>
#include <QFileInfo>

#include <QDebug>

#include "knmusicplaylistitem.h"
#include "knmusicplaylistlistmodel.h"
#include "knmusicnowplaying.h"
#include "../Libraries/knmusiclibrarymodel.h"
#include "../Libraries/knmusicinfocollector.h"
#include "../../knglobal.h"

#include "knmusicplaylistmanager.h"

KNMusicPlaylistManager::KNMusicPlaylistManager(QObject *parent) :
    KNMusicPlaylistManagerBase(parent)
{
    //Get the latest translation.
    retranslate();
    //Initial global settings.
    m_global=KNGlobal::instance();
    m_musicGlobal=KNMusicGlobal::instance();
    //Initial the icon.
    m_playlistIcon=QIcon(":/Music/Resources/Music/playlists/playlist.png");
    //Initial pathes.
    m_playlistPath=m_global->databaseFolder()+"/Playlists";
    QDir playlistDir(m_playlistPath);
    if(!playlistDir.exists())
    {
        playlistDir.mkpath(m_playlistPath);
    }
    //Here we should reset the playlist folder path,
    //Because on fucking Windows OS: the file path will like:
    //  D:\Qt\Projects\Nerve\Build\Desktop\Debug\debug\Library/Playlists/
    //So! We have to reset the file path again! because of the fucking Windows!
    m_playlistPath=playlistDir.absolutePath()+"/";
    m_configureFile=new QFile(m_playlistPath+"Playlists.db", this);
    //Initial category filter model.
    m_nowPlaying=new KNMusicNowPlaying(this);
    connect(m_nowPlaying, &KNMusicNowPlaying::requireUpdatePlaylistModel,
            this, &KNMusicPlaylistManager::requireUpdatePlaylistModel);
    //Initial playlist model.
    m_playlistListModel=new KNMusicPlaylistListModel(this);
    connect(m_playlistListModel, &KNMusicPlaylistListModel::itemChanged,
            this, &KNMusicPlaylistManager::onActionPlaylistItemChanged);
    //Initial info collector.
    m_infoCollector=new KNMusicInfoCollector(this);
    m_infoCollector->setSignalMode(false);
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
        QString currentFilePath=m_playlists.at(i).toString();
        QFile playlistFileTest(currentFilePath);
        if(playlistFileTest.exists())
        {
            KNMusicPlaylistItem *playlistItem=createPlaylistItem();
            playlistItem->setFilePath(currentFilePath);
            loadPlaylist(playlistItem);
            m_playlistListModel->appendRow(playlistItem);
        }
    }
}

void KNMusicPlaylistManager::savePlayLists()
{
    m_playlists=QJsonArray();
    for(int i=0; i<m_playlistListModel->rowCount(); i++)
    {
        KNMusicPlaylistItem *currentItem=
                static_cast<KNMusicPlaylistItem *>(m_playlistListModel->item(i));
        m_playlists.append(currentItem->filePath());
    }
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

QModelIndex KNMusicPlaylistManager::createPlaylist(const QString &title)
{
    //Prepare the playlist data.
    KNMusicPlaylistItem *playlistItem=createPlaylistItem();
    playlistItem->setText(title);
    //Prepare the file name.
    QString baseName=QDateTime::currentDateTime().toString("yyyyMMddhhmmsszzz"),
            playlistPath=m_playlistPath+
                         baseName+".nplst";
    QFileInfo playlistFile(playlistPath);
    int count=1;
    while(playlistFile.exists())
    {
        playlistPath=m_playlistPath+
                     baseName+" "+
                     QString::number(count++)+".nplst";
        playlistFile.setFile(playlistPath);
    }
    //Set the file name.
    playlistItem->setFilePath(playlistFile.absoluteFilePath());
    //Write to file.
    writePlaylist(playlistItem);
    //Create the model.
    buildPlaylist(playlistItem);
    //Add to playlist list.
    m_playlistListModel->appendRow(playlistItem);
    return playlistItem->index();
}

void KNMusicPlaylistManager::importPlaylist(QStringList filePaths)
{
    for(int i=0; i<filePaths.size(); i++)
    {
        QString currentPath=filePaths.at(i);
        //Process.
    }
}

void KNMusicPlaylistManager::removePlaylist(QString filePath)
{
    QModelIndexList indexSearch=m_playlistListModel->match(m_playlistListModel->index(0,0),
                                                       KNMusicPlaylistItem::PlaylistPath,
                                                       filePath);
    //If we cannot find the file, just return. Although this should never happen.
    if(indexSearch.isEmpty())
    {
        return;
    }
    if(filePath.contains(m_playlistPath))
    {
        //It's a path in playlist menu, delete it.
        QFile playlistFile(filePath);
        if(playlistFile.exists())
        {
            if(!playlistFile.remove())
            {
                qDebug()<<"Cannot Remove File!";
                return;
            }
        }
    }
    m_playlistListModel->removeRow(indexSearch.first().row());
}

QString KNMusicPlaylistManager::playlistPath(const int &index)
{
    if(index<0 || index>m_playlistListModel->rowCount())
    {
        return QString();
    }
    KNMusicPlaylistItem *currentItem=
               static_cast<KNMusicPlaylistItem *>(m_playlistListModel->item(index));
    return currentItem->filePath();
}

void KNMusicPlaylistManager::setPlaylist(const QString &filePath)
{
    QModelIndexList fileCheck=m_playlistListModel->match(m_playlistListModel->index(0,0),
                                                     KNMusicPlaylistItem::PlaylistPath,
                                                     filePath);
    if(fileCheck.isEmpty())
    {
        return;
    }
    KNMusicPlaylistItem *currentItem=
               static_cast<KNMusicPlaylistItem *>(m_playlistListModel->item(fileCheck.first().row()));
    m_nowPlaying->setPlaylist(currentItem->playlistModel());
}

void KNMusicPlaylistManager::onActionPlaylistItemChanged(QStandardItem *item)
{
    KNMusicPlaylistItem *currentItem=static_cast<KNMusicPlaylistItem *>(item);
    currentItem->setChanged(true);
    emit requireUpdateItem(item->index());
}

KNMusicPlaylistItem *KNMusicPlaylistManager::createPlaylistItem()
{
    KNMusicPlaylistItem *currentItem=new KNMusicPlaylistItem();
    currentItem->setIcon(m_playlistIcon);
    return currentItem;
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

bool KNMusicPlaylistManager::savePlaylist(KNMusicPlaylistItem *item)
{
    if(item->changed())
    {
        return writePlaylist(item);
    }
    return true;
}

bool KNMusicPlaylistManager::writePlaylist(KNMusicPlaylistItem *item)
{
    //Open the playlist file.
    QFile playlistFile(item->filePath());
    if(playlistFile.open(QIODevice::WriteOnly))
    {
        //Prepare the Json Data.
        QJsonObject playlistObject;
        playlistObject["Name"]=item->text();
        //Save the song model.
        QJsonArray songList;
        //Create Array
        for(int i=0; i<item->songCount();i++)
        {
            songList.append(item->songPath(i));
        }
        playlistObject["Songs"]=songList;
        QJsonDocument playlistContent;
        playlistContent.setObject(playlistObject);
        //Rewrite data.
        playlistFile.write(playlistContent.toJson());
        playlistFile.close();
        //Clear the data changed flag.
        item->setChanged(false);
        return true;
    }
    return false;
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
    for(int i=0, listSize=m_playlistListModel->rowCount();
        i<listSize;
        i++)
    {
        //If changed.
        KNMusicPlaylistItem *currentItem=
                static_cast<KNMusicPlaylistItem *>(m_playlistListModel->item(i, 0));
        savePlaylist(currentItem);
    }
}

int KNMusicPlaylistManager::loopMode()
{
    return m_nowPlaying->loopMode();
}

void KNMusicPlaylistManager::setBackend(KNMusicBackend *backend)
{
    m_infoCollector->setMusicBackend(backend);
}

void KNMusicPlaylistManager::setMusicModel(KNMusicLibraryModelBase *model)
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
    return index.isValid()?
                m_playlistListModel->data(index, Qt::DisplayRole).toString():
                QString();
}

QString KNMusicPlaylistManager::filePathFromIndex(const QModelIndex &index) const
{
    return m_nowPlaying->filePathFromIndex(index);
}

QAbstractItemModel *KNMusicPlaylistManager::playlistModel()
{
    return m_playlistListModel;
}

QAbstractItemModel *KNMusicPlaylistManager::playlistDataModel(const QModelIndex &index)
{
    if(index.isValid())
    {
        KNMusicPlaylistItem *currentItem=
                static_cast<KNMusicPlaylistItem *>(m_playlistListModel->itemFromIndex(index));
        if(currentItem->modelBuild())
        {
            return currentItem->playlistModel();
        }
        return buildPlaylist(currentItem);
    }
    return nullptr;
}

void KNMusicPlaylistManager::retranslate()
{
    ;
}

void KNMusicPlaylistManager::retranslateAndSet()
{
    ;
}
