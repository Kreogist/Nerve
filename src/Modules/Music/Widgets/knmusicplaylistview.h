#ifndef KNMUSICPLAYLISTVIEW_H
#define KNMUSICPLAYLISTVIEW_H

#include <QSplitter>

class QAbstractItemModel;
class KNMusicPlaylistListview;
class KNMusicPlaylistListEditor;
class KNMusicPlaylistManagerBase;
class KNMusicPlaylistDisplay;
class KNMusicPlaylistSongs;
class KNMusicPlaylistView : public QSplitter
{
    Q_OBJECT
public:
    explicit KNMusicPlaylistView(QWidget *parent = 0);
    void setManager(KNMusicPlaylistManagerBase *manager);

signals:
    void requirePlayMusic(const QString &filePath);

public slots:

private slots:
    void onActionShowPlaylist(const QModelIndex &index);
    void onActionOpenUrl(const QModelIndex &index);

private:
    KNMusicPlaylistManagerBase *m_manager;
    KNMusicPlaylistListview *m_playlistListView;
    KNMusicPlaylistListEditor *m_playlistListEditor;
    KNMusicPlaylistDisplay *m_displayer;
    KNMusicPlaylistSongs *m_songsView;
    QString m_currentPath;
};

#endif // KNMUSICPLAYLISTVIEW_H
