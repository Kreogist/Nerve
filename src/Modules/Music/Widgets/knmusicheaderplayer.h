#ifndef KNMUSICHEADERPLAYER_H
#define KNMUSICHEADERPLAYER_H

#include <QWidget>

class QLabel;
class KNPlayerProgress;
class KNMusicPlayer;
class KNMusicVisualEffect;
class KNMusicHeaderPlayer : public QWidget
{
    Q_OBJECT
public:
    explicit KNMusicHeaderPlayer(QWidget *parent = 0);
    QWidget *visualEffect();
    void setAlbumArt(const QPixmap &albumArt);
    void setTitle(const QString &string);
    void setArtist(const QString &string);
    void setPlayer(KNMusicPlayer *player);
    void playFile(const QString &filePath);

signals:

public slots:

private slots:
    void onActionPositonChanged(const int &position);
    void onActionDurationChanged(const int &duration);
    void onActionReachEndOfMusic();

private:
    void resetPosition();
    QLabel *m_albumArt, *m_title, *m_artist;
    KNPlayerProgress *m_progress;
    QPalette m_textPalette;
    KNMusicPlayer *m_player;
    bool m_sliderPressed=false;
    KNMusicVisualEffect *m_visualEffect;
};

#endif // KNMUSICHEADERPLAYER_H
