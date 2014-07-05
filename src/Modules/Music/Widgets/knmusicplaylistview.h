#ifndef KNMUSICPLAYLISTVIEW_H
#define KNMUSICPLAYLISTVIEW_H

#include <QSplitter>

class QBoxLayout;
class QAbstractItemModel;
class KNMusicPlaylistListView;
class KNMusicPlaylistListEditorBase;
class KNMusicPlaylistManagerBase;
class KNMusicPlaylistDisplay;
class KNMusicPlaylistSongs;
class KNMusicPlaylistView : public QSplitter
{
    Q_OBJECT
public:
    explicit KNMusicPlaylistView(QWidget *parent = 0);
    void setManager(KNMusicPlaylistManagerBase *manager);
    void setListEditor(KNMusicPlaylistListEditorBase *editor);

signals:
    void requirePlayMusic(const QString &filePath);

public slots:
    void onActionAddPlaylist();
    void onActionRemoveCurrentPlaylist();
    void onActionEditPlaylist(const QModelIndex &index);

private slots:
    void onActionShowPlaylist(const QModelIndex &index);
    void onActionOpenUrl(const QModelIndex &index);
    void onActionItemUpdate(const QModelIndex &index);

private:
    KNMusicPlaylistManagerBase *m_manager;
    KNMusicPlaylistListView *m_playlistListView;
    KNMusicPlaylistListEditorBase *m_playlistListEditor;
    KNMusicPlaylistDisplay *m_displayer;
    KNMusicPlaylistSongs *m_songsView;
    QBoxLayout *m_listLayout;
    QString m_currentPath;
};

#endif // KNMUSICPLAYLISTVIEW_H
