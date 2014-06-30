#ifndef KNMUSICPLAYLISTMANAGERBASE_H
#define KNMUSICPLAYLISTMANAGERBASE_H

#include <QStringList>

#include <QObject>

class QAbstractItemModel;
class QSortFilterProxyModel;
class QStandardItemModel;
class QFile;
class KNMusicBackend;
class KNMusicInfoCollector;
class KNMusicNowPlaying;
class KNMusicPlaylistItem;
class KNMusicLibraryModelBase;
class KNMusicCategoryDetailModel;
class KNMusicPlaylistManagerBase : public QObject
{
    Q_OBJECT
public:
    explicit KNMusicPlaylistManagerBase(QObject *parent = 0);
    //Playlist lists management
    virtual void loadPlayLists()=0;
    virtual void savePlayLists()=0;
    virtual int loopMode()=0;

    //Backend sets.
    virtual void setBackend(KNMusicBackend *backend)=0;

    //Music model.
    virtual void setMusicModel(KNMusicLibraryModelBase *model)=0;

    virtual void setProxyModel(QSortFilterProxyModel *model)=0;
    virtual void setCurrentPlaying(const QString &string)=0;
    virtual void setCurrentPlaylistPlaying(const QModelIndex &index)=0;
    virtual void addToCurrentList(const QString &index)=0;

    //Song acquirment functions.
    virtual QString nextPlayingSong()=0;
    virtual QString nextSong()=0;
    virtual QString prevSong()=0;
    virtual QString playlistName(const QModelIndex &index) const=0;
    virtual QString filePathFromIndex(const QModelIndex &index) const=0;

    //Datas ports.
    virtual QAbstractItemModel *playlistModel()=0;
    virtual QAbstractItemModel *playlistDataModel(const QModelIndex &index)=0;


signals:
    void requireUpdatePlaylistModel(QAbstractItemModel *playlist);
    void playlistListUpdated();

public slots:
    virtual void setLoopMode(const int &index)=0;
    //Create playlist
    virtual void createPlaylist(const QString &title)=0;
    virtual void importPlaylist(QStringList filePaths)=0;
    virtual void removePlaylist(const int &index)=0;
    virtual QString setModelPlaylist(const int &index)=0;
    virtual void setPlaylist(const QString &filePath)=0;

};

#endif // KNMUSICPLAYLISTMANAGERBASE_H
