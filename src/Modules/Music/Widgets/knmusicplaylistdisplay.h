#ifndef KNMUSICPLAYLISTDISPLAY_H
#define KNMUSICPLAYLISTDISPLAY_H

#include <QWidget>

class QLabel;
class QBoxLayout;
class KNMusicPlaylistSongs;
class KNMusicPlaylistDisplay : public QWidget
{
    Q_OBJECT
public:
    explicit KNMusicPlaylistDisplay(QWidget *parent = 0);
    void setSongsView(KNMusicPlaylistSongs *songsView);

signals:

public slots:

private:
    QLabel *m_playlistName, *m_playlistSongCount;
    QBoxLayout *m_layout;
    KNMusicPlaylistSongs *m_songsView;
};

#endif // KNMUSICPLAYLISTDISPLAY_H
