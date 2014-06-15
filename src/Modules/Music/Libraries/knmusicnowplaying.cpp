#include "../Libraries/knmusicmodel.h"
#include "../Libraries/knmusiccategorydetailmodel.h"

#include "knmusicnowplaying.h"

KNMusicNowPlaying::KNMusicNowPlaying(QObject *parent) :
    KNMusicAbstractPlaylist(parent)
{
    m_categoryProxyModel=new KNMusicCategoryDetailModel(this);
}

void KNMusicNowPlaying::setMusicModel(KNMusicModel *model)
{
    m_musicModel=model;
    m_categoryProxyModel->setSourceModel(m_musicModel);
}

void KNMusicNowPlaying::setProxyModel(QSortFilterProxyModel *model)
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
bool KNMusicNowPlaying::usingProxy() const
{
    return m_usingProxy;
}

QString KNMusicNowPlaying::prevSong()
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
    return m_currentPath;
}

QString KNMusicNowPlaying::nextSong()
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

int KNMusicNowPlaying::loopMode()
{
    return m_loopMode;
}

void KNMusicNowPlaying::setCurrentPlaying(const QString &string)
{
    //If using a proxy, search in the proxy model.
    if(m_usingProxy)
    {
        m_currentPath=string;
        return;
    }
    //Or else, search in the playlist.
    m_currentPath=string;
}

void KNMusicNowPlaying::setLoopMode(const int &index)
{
    m_loopMode=index;
}

