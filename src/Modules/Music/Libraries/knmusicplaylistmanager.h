#ifndef KNMUSICPLAYLISTMANAGER_H
#define KNMUSICPLAYLISTMANAGER_H

#include <QList>
#include <QObject>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonParseError>
#include <QJsonArray>
#include <QPixmap>

#include "../../knglobal.h"
#include "../knmusicglobal.h"

class QSortFilterProxyModel;
class QStandardItemModel;
class QFile;
class KNLibBass;
class KNMusicInfoCollectorManager;
class KNMusicNowPlaying;
class KNMusicPlaylistItem;
class KNMusicModel;
class KNMusicCategoryDetailModel;
class KNMusicPlaylistManager : public QObject
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
    void setMusicBackend(KNLibBass *backend);
    void setMusicModel(KNMusicModel *model);
    void setProxyModel(QSortFilterProxyModel *model);
    void setCurrentPlaying(const QString &string);
    void addToCurrentList(const QString &index);

    //Song acquirment functions.
    QString nextPlayingSong();
    QString nextSong();
    QString prevSong();

    //Datas ports.
    QAbstractItemModel *playlistModel();
    QAbstractItemModel *playlistDataModel();

signals:
    void requireUpdatePlaylistModel(QAbstractItemModel *playlist);
    void playlistListUpdated();

public slots:
    void retranslate();
    void retranslateAndSet();
    void setLoopMode(const int &index);
    //Create playlist
    void createPlaylist(const QString &title);
    void importPlaylist(QStringList filePaths);
    void removePlaylist(const int &index);
    void setPlaylist(const int &index);

private:
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
    KNMusicModel *m_musicModel,
                 *m_playlistDataModel;
    KNMusicInfoCollectorManager *m_infoCollectManager;
    QStandardItemModel *m_playlistModel;

    //Global instance.
    KNMusicGlobal *m_musicGlobal;
    KNGlobal *m_global;
};

#endif // KNMUSICPLAYLISTMANAGER_H
