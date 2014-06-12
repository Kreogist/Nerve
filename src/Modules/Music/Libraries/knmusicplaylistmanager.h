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
class QFile;
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
    void setMusicModel(KNMusicModel *model);
    void setProxyModel(QSortFilterProxyModel *model);
    void playNow(const QString &string);
    void addToCurrentList(const QString &index);

    //Song acquirment functions.
    QString nextSong();
    QString nextListSong();
    QString prevListSong();

    //Datas ports.
    QStringList playlistNameList() const;

signals:
    void playlistListUpdated();
    void requireUpdatePlaylistModel(QAbstractItemModel *playlistModel);

public slots:
    void retranslate();
    void retranslateAndSet();
    void setLoopMode(const int &index);
    //Create playlist
    void createPlaylist(const QString &title);
    void importPlaylist(QStringList filePaths);
    void removePlaylist(const int &index);

private:
    QString currentIndexPath();
    void saveAllChanged();
    void savePlayList(const int &index);
    bool loadPlayList(const QString &filePath);
    //Playlist-List data.
    struct KNPlayList
    {
        QStringList songs;
        //The last playing index is only for reference.
        int lastPlayingIndex=-1;
        bool changed=false;
    };
    QStringList m_playlistNameList;
    QList<KNPlayList> m_playlistList;
    //Playlist-List read & write data.
    QJsonDocument m_configureContent;
    QJsonObject m_configure;
    QJsonArray m_playlists;
    QJsonParseError m_error;
    QFile *m_configureFile;

    //Now playing playlist
    KNPlayList m_nowPlayingList;
    //Current playlist pointer
    bool m_usingProxy=false;
    int m_loopMode=KNMusicGlobal::NoRepeat;
    KNPlayList *m_currentList;
    QString m_currentPath, m_playlistPath;

    //Music Database and backends
    KNMusicModel *m_musicModel;
    QSortFilterProxyModel *m_proxyModel;
    KNMusicCategoryDetailModel *m_categoryProxyModel;

    //Global instance.
    KNMusicGlobal *m_musicGlobal;
    KNGlobal *m_global;
};

#endif // KNMUSICPLAYLISTMANAGER_H
