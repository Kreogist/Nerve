#include <QDebug>

#include "../Libraries/knmusiclibrarymodel.h"
#include "../Libraries/knmusiccategorydetailmodel.h"

#include "knmusicplaylistmodel.h"

#include "knmusicnowplaying.h"

KNMusicNowPlaying::KNMusicNowPlaying(QObject *parent) :
    KNMusicAbstractPlaylist(parent)
{
    m_categoryProxyModel=new KNMusicCategoryDetailModel(this);
}

void KNMusicNowPlaying::setMusicModel(KNMusicLibraryModelBase *model)
{
    m_musicModel=model;
    m_categoryProxyModel->setSourceModel(m_musicModel);
}

void KNMusicNowPlaying::setProxyModel(QSortFilterProxyModel *model)
{
    m_mode=ProxyModelMode;
    if(model->filterRegExp().pattern().isEmpty())
    {
        m_proxyModel=model;
    }
    else
    {
        m_categoryProxyModel->setFilterRegExp(model->filterRegExp());
        m_categoryProxyModel->setSortCaseSensitivity(model->sortCaseSensitivity());
        m_categoryProxyModel->setSortRole(model->sortRole());
        m_categoryProxyModel->setFilterKeyColumn(model->filterKeyColumn());
        m_categoryProxyModel->sort(model->sortColumn(), model->sortOrder());
        m_proxyModel=m_categoryProxyModel;
    }
    setCurrentModel(m_proxyModel);
}

QString KNMusicNowPlaying::prevSong()
{
    int nowPlayingIndex;
    //If we are using proxy mode, search in the proxy model.
    switch(m_mode)
    {
    case ProxyModelMode:
        //If current path is empty, set to the initial position.
        if(m_currentPath.isEmpty())
        {
            m_currentPath=
                    m_musicModel->filePathFromIndex(
                        m_proxyModel->mapToSource(m_proxyModel->index(m_proxyModel->rowCount()-1,
                                                                      0)));
            return m_currentPath;
        }
        //Switch to the next according to the loop mode.
        nowPlayingIndex=prevSongRow(m_proxyModel->mapFromSource(m_musicModel->indexFromFilePath(m_currentPath)).row(),
                                    m_proxyModel->rowCount());
        //Translate back to file path.
        m_currentPath=nowPlayingIndex==-1?
                      QString():
                        m_musicModel->filePathFromIndex(
                        m_proxyModel->mapToSource(m_proxyModel->index(nowPlayingIndex,0)));
        break;
    case PlayListMode:
        if(m_playlist==nullptr)
        {
            m_currentItem=nullptr;
            m_currentPath.clear();
            return m_currentPath;
        }
        //If current path is empty, set to the initial position.
        if(m_currentPath.isEmpty())
        {
            nowPlayingIndex=m_playlist->rowCount()-1;
            m_currentItem=m_playlist->item(nowPlayingIndex,0);
            m_currentPath=m_playlist->filePathFromIndex(m_playlist->index(nowPlayingIndex,0));
            return m_currentPath;
        }
        nowPlayingIndex=prevSongRow(m_currentItem->row(),
                                    m_playlist->rowCount());
        if(nowPlayingIndex==-1)
        {
            m_currentItem=nullptr;
            m_currentPath.clear();
        }
        else
        {
            m_currentItem=m_playlist->item(nowPlayingIndex, 0);
            m_currentPath=m_playlist->filePathFromIndex(nowPlayingIndex);
        }
        break;
    default:
        break;
    }
    return m_currentPath;
}

QString KNMusicNowPlaying::nextSong()
{
    int nowPlayingIndex;
    //If we are using proxy mode, search in the proxy model.
    switch(m_mode)
    {
    case ProxyModelMode:
        //If current path is empty, set to the initial position.
        if(m_currentPath.isEmpty())
        {
            m_currentPath=
                    m_musicModel->filePathFromIndex(
                        m_proxyModel->mapToSource(m_proxyModel->index(0,0)));
            return m_currentPath;
        }
        //Translate to index.
        nowPlayingIndex=nextSongRow(m_proxyModel->mapFromSource(m_musicModel->indexFromFilePath(m_currentPath)).row(),
                                    m_proxyModel->rowCount());
        //Translate back to file path.
        m_currentPath=nowPlayingIndex==-1?
                      QString():
                        m_musicModel->filePathFromIndex(
                        m_proxyModel->mapToSource(m_proxyModel->index(nowPlayingIndex,0)));
        break;
    case PlayListMode:
        //Check the playlist model is null or not. If it's null, means the
        //current model has been delete.
        if(m_playlist==nullptr)
        {
            m_currentItem=nullptr;
            m_currentPath.clear();
            return m_currentPath;
        }
        //If current path is empty, set to the initial position.
        if(m_currentPath.isEmpty())
        {
            m_currentItem=m_playlist->item(0,0);
            m_currentPath=m_playlist->filePathFromIndex(m_playlist->index(0,0));
            return m_currentPath;
        }
        nowPlayingIndex=nextSongRow(m_currentItem->row(),
                                    m_playlist->rowCount());
        if(nowPlayingIndex==-1)
        {
            m_currentItem=nullptr;
            m_currentPath.clear();
        }
        else
        {
            m_currentItem=m_playlist->item(nowPlayingIndex, 0);
            m_currentPath=m_playlist->filePathFromIndex(nowPlayingIndex);
        }
        break;
    default:
        break;
    }
    //Means using the now playing list.
    return m_currentPath;
}

QString KNMusicNowPlaying::nextPlayingSong()
{
    /*! FIXME:
      Judge whether there's songs wait for playing next*/
    if(m_loopMode==KNMusicGlobal::RepeatSong)
    {
        return m_currentPath;
    }
    return nextSong();
}

QString KNMusicNowPlaying::filePathFromIndex(const QModelIndex &index)
{
    if(m_mode==ProxyModelMode)
    {
        return QString();
    }
    return m_playlist->filePathFromIndex(index);
}

int KNMusicNowPlaying::loopMode()
{
    return m_loopMode;
}

void KNMusicNowPlaying::setNowPlaying(const QString &string)
{
    switch(m_mode)
    {
    case ProxyModelMode:
        //If using a proxy, search in the proxy model.
        m_currentPath=string;
        break;
    case TemporaryListMode:
        //Then it means: we cannot find the file anywhere?! Might a new file from
        //disk, create a temp playlist for it.
        m_temporaryPlaylist=QStringList(string);
        ;
    }
    //    m_playlist=m_temporaryPlaylist;
}

void KNMusicNowPlaying::setNowPlaying(const QModelIndex &index)
{
    //When calling this, this should must be in Playlist Mode.
    if(m_mode==PlayListMode)
    {
        m_currentItem=m_playlist->item(index.row(), 0);
        m_currentPath=m_playlist->filePathFromIndex(index);
        return;
    }
    m_currentItem=nullptr;
    m_currentPath.clear();
}

void KNMusicNowPlaying::setLoopMode(const int &index)
{
    m_loopMode=index;
}

int KNMusicNowPlaying::nextSongRow(int currentRow, int rowCount)
{
    int nowPlayingIndex=currentRow;
    //Switch to the next according to the loop mode.
    switch(m_loopMode)
    {
    case KNMusicGlobal::RepeatAll:
        nowPlayingIndex=nowPlayingIndex==rowCount-1?
                    0:nowPlayingIndex+1;
        break;
    case KNMusicGlobal::RepeatSong:
    case KNMusicGlobal::NoRepeat:
        if(nowPlayingIndex==rowCount-1)
        {
            return -1;
        }
        nowPlayingIndex++;
        break;
    }
    return nowPlayingIndex;
}

int KNMusicNowPlaying::prevSongRow(int currentRow, int rowCount)
{
    int nowPlayingIndex=currentRow;
    switch(m_loopMode)
    {
    case KNMusicGlobal::RepeatAll:
        nowPlayingIndex=nowPlayingIndex==0?
                    rowCount-1:nowPlayingIndex-1;
        break;
    case KNMusicGlobal::RepeatSong:
    case KNMusicGlobal::NoRepeat:
        if(nowPlayingIndex==0)
        {
            return -1;
        }
        nowPlayingIndex--;
        break;
    }
    return nowPlayingIndex;
}

void KNMusicNowPlaying::setCurrentModel(QAbstractItemModel *playlistModel)
{
    m_currentModel=playlistModel;
    emit requireUpdatePlaylistModel(m_currentModel);
}

QString KNMusicNowPlaying::playlistPath() const
{
    return m_playlistPath;
}

void KNMusicNowPlaying::resetPlaylistModel()
{
    m_playlistPath=QString();
    m_playlist=nullptr;
    setCurrentModel(m_playlist);
}

void KNMusicNowPlaying::setPlaylistPath(const QString &playlistPath)
{
    m_playlistPath=playlistPath;
}

void KNMusicNowPlaying::setPlaylist(QAbstractItemModel *playlistModel)
{
    //Disable proxy from database.
    m_mode=PlayListMode;
    //Clear the current playing data.
    m_currentPath.clear();
    //Set the playlist pointer.
    m_playlist=static_cast<KNMusicPlaylistModel *>(playlistModel);
    //Set the current model.
    setCurrentModel(m_playlist);
}

