#ifndef KNMUSICPLAYLISTITEM_H
#define KNMUSICPLAYLISTITEM_H

#include <QList>

#include <QStandardItem>

class QStandardItemModel;
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
    QStandardItemModel *playlistModel();
    void clearSongPaths();
    QStringList songPaths() const;
    void appendSongRow(const QList<QStandardItem *> &rowData);
    void setSongPaths(const QStringList &songPaths);

private:
    void resetPlaylist(const QString &fileName=QString());
    QStandardItemModel *m_playlistModel;
    QStringList m_songPaths;
};

#endif // KNMUSICPLAYLISTITEM_H
