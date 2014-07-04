#ifndef KNMUSICPLAYLISTMANAGER_H
#define KNMUSICPLAYLISTMANAGER_H

#include <QList>
#include <QUrl>
#include <QIcon>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonParseError>
#include <QJsonArray>
#include <QThread>
#include <QPixmap>

#include "../knmusicglobal.h"
#include "../../knglobal.h"

#include "../Base/knmusicplaylistmanagerbase.h"

class QStandardItem;
class KNMusicPlaylistListModel;
class KNMusicPlaylistManager : public KNMusicPlaylistManagerBase
{
    Q_OBJECT
public:
    explicit KNMusicPlaylistManager(QObject *parent = 0);
    ~KNMusicPlaylistManager();
    //Playlist lists management
    void loadPlayLists();
    void savePlayLists();
    int loopMode();

    //Backend sets.
    void setMusicBackend(KNMusicBackend *backend);
    void setMusicModel(KNMusicLibraryModelBase *model);
    void setProxyModel(QSortFilterProxyModel *model);
    void setCurrentPlaying(const QString &string);
    void setCurrentPlaylistPlaying(const QModelIndex &index);
    void addToCurrentList(const QString &index);

    //Song acquirment functions.
    QString nextPlayingSong();
    QString nextSong();
    QString prevSong();
    QString playlistName(const QModelIndex &index) const;
    QString filePathFromIndex(const QModelIndex &index) const;

    //Datas ports.
    QAbstractItemModel *playlistModel();
    QAbstractItemModel *playlistDataModel(const QModelIndex &index);

signals:

public slots:
    void retranslate();
    void retranslateAndSet();
    void setLoopMode(const int &index);
    //Create playlist
    QModelIndex createPlaylist(const QString &title);
    void importPlaylist(QStringList filePaths);
    void removePlaylist(QString filePath);
    QString playlistPath(const int &index);
    void setPlaylist(const QString &filePath);

private slots:
    void onActionPlaylistItemChanged(QStandardItem *item);
    void onActionAddToPlaylist(const int &row,
                               const QList<QUrl> &fileList);
    void onActionCreateList(const QList<QUrl> &fileList);

private:
    KNMusicPlaylistItem *createPlaylistItem();
    bool loadPlaylist(KNMusicPlaylistItem *item);
    bool savePlaylist(KNMusicPlaylistItem *item);
    bool writePlaylist(KNMusicPlaylistItem *item);
    QAbstractItemModel *buildPlaylist(KNMusicPlaylistItem *item);
    void addSongToPlaylist(KNMusicPlaylistItem *item,
                           const QString &filePath);
    QString currentIndexPath();
    void saveAllChanged();
    //Playlist-List read & write data.
    QJsonDocument m_configureContent;
    QJsonObject m_configure;
    QJsonArray m_playlists;
    QJsonParseError m_error;
    QFile *m_configureFile;

    //Now playing playlist
    KNMusicNowPlaying *m_nowPlaying;

    //Current playlist pointer
    QString m_currentPath, m_playlistPath;
    KNMusicLibraryModelBase *m_musicModel;
    KNMusicPlaylistListModel *m_playlistListModel;
    QThread m_dataModelThread, m_infoThread;
    KNMusicInfoCollector *m_infoCollector;

    //Global instance.
    KNMusicGlobal *m_musicGlobal;
    KNGlobal *m_global;

    //Icons
    QIcon m_playlistIcon;
};

#endif // KNMUSICPLAYLISTMANAGER_H
