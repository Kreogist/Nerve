#ifndef KNMUSICHEADERPLAYER_H
#define KNMUSICHEADERPLAYER_H

#include <QWidget>

class QLabel;
class QPropertyAnimation;
class KNPlayerProgress;
class KNMusicPlayer;
class KNMusicVisualEffect;
class KNMusicPlayerControl;
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

protected:
    void enterEvent(QEvent *event);
    void leaveEvent(QEvent *event);
    void resizeEvent(QResizeEvent *event);

private slots:
    void onActionPositonChanged(const int &position);
    void onActionDurationChanged(const int &duration);
    void onActionReachEndOfMusic();
    void onActionMouseInOut(const QVariant &controlPos);

private:
    void resetPosition();
    QLabel *m_albumArt, *m_title, *m_artist;
    KNPlayerProgress *m_progress;
    QPalette m_textPalette;
    KNMusicPlayer *m_player;
    KNMusicPlayerControl *m_playerControl;
    QPropertyAnimation *m_mouseIn, *m_mouseOut;
    bool m_sliderPressed=false;
    KNMusicVisualEffect *m_visualEffect;
};

#endif // KNMUSICHEADERPLAYER_H
