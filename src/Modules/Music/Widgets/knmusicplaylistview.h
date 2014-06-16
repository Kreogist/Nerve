#ifndef KNMUSICPLAYLISTVIEW_H
#define KNMUSICPLAYLISTVIEW_H

#include <QSplitter>

class QAbstractItemModel;
class KNMusicPlaylistListview;
class KNMusicPlaylistManager;
class KNMusicPlaylistDisplay;
class KNMusicPlaylistSongs;
class KNMusicPlaylistView : public QSplitter
{
    Q_OBJECT
public:
    explicit KNMusicPlaylistView(QWidget *parent = 0);
    void setManager(KNMusicPlaylistManager *manager);

signals:

public slots:

private slots:
    void onActionShowPlaylist(const QModelIndex &index);

private:
    KNMusicPlaylistManager *m_manager;
    KNMusicPlaylistListview *m_playlistListView;
    KNMusicPlaylistDisplay *m_displayer;
    KNMusicPlaylistSongs *m_songsView;
};

#endif // KNMUSICPLAYLISTVIEW_H
