#ifndef KNMUSICNOWPLAYING_H
#define KNMUSICNOWPLAYING_H

#include <QStringList>

#include "../knmusicglobal.h"

#include "knmusicabstractplaylist.h"

class QSortFilterProxyModel;
class QStandardItemModel;
class KNMusicModel;
class KNMusicCategoryDetailModel;
class KNMusicNowPlaying : public KNMusicAbstractPlaylist
{
    Q_OBJECT
public:
    explicit KNMusicNowPlaying(QObject *parent = 0);
    void setMusicModel(KNMusicModel *model);
    void setProxyModel(QSortFilterProxyModel *model);
    bool usingProxy() const;
    QString prevSong();
    QString nextSong();
    QString nextPlayingSong();
    int loopMode();
    void setCurrentPlaying(const QString &string);
    QAbstractItemModel *playlist() const;
    void setPlaylist(QAbstractItemModel *playlistModel);

signals:
    void requireUpdatePlaylistModel(QAbstractItemModel *playlistModel);

public slots:
    void setLoopMode(const int &index);

private:
    int m_loopMode=KNMusicGlobal::NoRepeat;
    KNMusicModel *m_musicModel;
    QSortFilterProxyModel *m_proxyModel;
    QString m_currentPath;
    QStandardItemModel *m_playlist;
    QStringList m_temporaryPlaylist;
    KNMusicCategoryDetailModel *m_categoryProxyModel;
    bool m_usingProxy=false;
};

#endif // KNMUSICNOWPLAYING_H
