#ifndef KNMUSICPLAYLISTVIEW_H
#define KNMUSICPLAYLISTVIEW_H

#include <QSplitter>

class KNMusicFileExplorer;
class KNMusicPlaylistListEditor;
class KNMusicPlaylistManager;
class KNMusicPlaylistListview;
class KNMusicPlaylistView : public QSplitter
{
    Q_OBJECT
public:
    explicit KNMusicPlaylistView(QWidget *parent = 0);
    void setPlaylistManager(KNMusicPlaylistManager *manager);

signals:

public slots:
    void retranslate();
    void retranslateAndSet();

private slots:
    void onActionUpdatePlaylists();
    void onActionCreatePlaylist();
    void onActionOpenPlaylist();
    void onActionRemoveCurrent();

private:
    enum Headers
    {
        Playlist,
        Dirs,
        HeaderCount
    };
    QString m_headerCaption[HeaderCount];
    int m_headerIndex[HeaderCount];
    KNMusicPlaylistManager *m_playlistManager;
    KNMusicPlaylistListEditor *m_listEditor;
    KNMusicPlaylistListview *m_categoryList;
    QWidget *m_playlistViewer;
    KNMusicFileExplorer *m_dirViewer;
};

#endif // KNMUSICPLAYLISTVIEW_H
