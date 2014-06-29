#ifndef KNMUSICNOWPLAYING_H
#define KNMUSICNOWPLAYING_H

#include <QStringList>

#include "../knmusicglobal.h"

#include "knmusicabstractplaylist.h"

class QSortFilterProxyModel;
class QStandardItemModel;
class QStandardItem;
class KNMusicLibraryModelBase;
class KNMusicPlaylistModel;
class KNMusicCategoryDetailModel;
class KNMusicNowPlaying : public KNMusicAbstractPlaylist
{
    Q_OBJECT
public:
    explicit KNMusicNowPlaying(QObject *parent = 0);
    void setMusicModel(KNMusicLibraryModelBase *model);
    void setProxyModel(QSortFilterProxyModel *model);
    QString prevSong();
    QString nextSong();
    QString nextPlayingSong();
    QString filePathFromIndex(const QModelIndex &index);
    int loopMode();
    void setCurrentPlaying(const QString &string);
    void setCurrentPlaying(const QModelIndex &index);
    QAbstractItemModel *playlist() const;
    void setPlaylist(QAbstractItemModel *playlistModel);

signals:
    void requireUpdatePlaylistModel(QAbstractItemModel *playlistModel);

public slots:
    void setLoopMode(const int &index);

private:
    enum NowPlayingMode
    {
        NoListMode,
        ProxyModelMode,
        PlayListMode,
        TemporaryListMode
    };
    int nextSongRow(int currentRow, int rowCount);
    int prevSongRow(int currentRow, int rowCount);

    int m_loopMode=KNMusicGlobal::NoRepeat,
        m_mode=NoListMode;
    KNMusicLibraryModelBase *m_musicModel;
    QSortFilterProxyModel *m_proxyModel;
    QString m_currentPath;
    QStandardItem *m_currentItem;
    QAbstractItemModel *m_currentModel;
    KNMusicPlaylistModel *m_playlist;
    QStringList m_temporaryPlaylist;
    KNMusicCategoryDetailModel *m_categoryProxyModel;
};

#endif // KNMUSICNOWPLAYING_H
