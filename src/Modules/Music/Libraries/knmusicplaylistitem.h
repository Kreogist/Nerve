#ifndef KNMUSICPLAYLISTITEM_H
#define KNMUSICPLAYLISTITEM_H

#include <QList>

#include <QStandardItem>

class KNMusicPlaylistItem : public QStandardItem
{
public:
    enum PlayListProperties
    {
        FilePath=Qt::UserRole,
        SongsPath,
        DataChanged
    };

    explicit KNMusicPlaylistItem();
    ~KNMusicPlaylistItem();
    void setName(const QString &name);
    bool changed();
    bool removedFile();
    QString filePath() const;
    void setFilePath(const QString &path);
    void setPlaylist(const QStringList &data);
    QStringList playlist() const;
    bool loadPlaylist();
    bool savePlayList();
    bool forceSavePlaylist();

private:
    void resetPlaylist(const QString &fileName=QString());
};

#endif // KNMUSICPLAYLISTITEM_H
