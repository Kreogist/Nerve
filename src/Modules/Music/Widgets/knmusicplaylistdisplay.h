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
    void setPlaylistName(const QString &name);
    void setSongCount(const int &songCount);
    void setSongsView(KNMusicPlaylistSongs *songsView);

signals:

public slots:
    void retranslate();
    void retranslateAndSet();

private:
    QLabel *m_playlistName, *m_playlistSongCount;
    QString m_songCaption, m_songsCaption;
    QBoxLayout *m_layout;
    KNMusicPlaylistSongs *m_songsView;
};

#endif // KNMUSICPLAYLISTDISPLAY_H
