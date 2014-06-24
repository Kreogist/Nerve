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
        FilePath=Qt::UserRole,
        DataChanged,
        ModelBuild
    };

    explicit KNMusicPlaylistItem();
    ~KNMusicPlaylistItem();
    void setName(const QString &name);
    bool changed();
    bool removedFile();
    QString filePath() const;
    void setFilePath(const QString &path);
    bool savePlayList();
    bool forceSavePlaylist();
    bool modelBuild() const;
    QModelIndex firstIndexFromMusicPath(const QString &filePath);
    QList<QStandardItem *> songRow(const int &row) const;
    QStandardItemModel *playlistModel();
    void clearSongPaths();
    QStringList songPaths() const;
    void appendSongRow(const QList<QStandardItem *> &rowData);
    void appendSongItem(QStringList textList,
                        KNMusicGlobal::MusicDetailsInfo currentDetails);
    void setSongPaths(const QStringList &songPaths);

private:
    void resetPlaylist(const QString &fileName=QString());
    KNMusicPlaylistModel *m_playlistModel;
    QStringList m_songPaths;
};

#endif // KNMUSICPLAYLISTITEM_H
