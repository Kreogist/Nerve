#ifndef KNMUSICPLAYLISTITEM_H
#define KNMUSICPLAYLISTITEM_H

#include <QList>

#include <QStandardItem>

#include "../knmusicglobal.h"

class QStandardItemModel;
class KNMusicPlaylistModel;
class KNMusicPlaylistItem : public QStandardItem
{
public:
    enum PlayListProperties
    {
        PlaylistPath=Qt::UserRole,
        DataChanged,
        ModelBuild
    };

    explicit KNMusicPlaylistItem();
    ~KNMusicPlaylistItem();
    void setName(const QString &name);
    bool changed();
    void setChanged(const bool &changed);
    QString filePath() const;
    void setFilePath(const QString &path);
    bool modelBuild() const;
    QModelIndex firstIndexFromMusicPath(const QString &filePath);
    QList<QStandardItem *> songRow(const int &row) const;
    QStandardItemModel *playlistModel();
    void clearSongPaths();
    QStringList songPaths() const;
    void appendSongRow(const QList<QStandardItem *> &rowData);
    void appendSongItem(QStringList textList,
                        KNMusicGlobal::MusicDetailsInfo currentDetails);
    int songCount() const;
    QString songPath(const int &songIndex) const;
    void setSongPaths(const QStringList &songPaths);

private:
    void resetPlaylist(const QString &fileName=QString());
    KNMusicPlaylistModel *m_playlistModel;
    QStringList m_songPaths;
    bool m_dataChanged=false;
};

#endif // KNMUSICPLAYLISTITEM_H
