#ifndef KNMUSICHEADERPLAYER_H
#define KNMUSICHEADERPLAYER_H

#include <QWidget>
#include <QLabel>

class QPropertyAnimation;
class QMouseEvent;
class KNPlayerProgress;
class KNLabelEditor;
class KNMusicPlayer;
class KNMusicVisualEffect;
class KNMusicPlayerControl;

class KNMusicHeaderAlbumArt : public QLabel
{
    Q_OBJECT
public:
    explicit KNMusicHeaderAlbumArt(QWidget *parent = 0);

signals:
    void requireShowInCurrent();

protected:
    void mousePressEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);

private:
    bool m_isPressed;
};

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
    void requireShowInCurrent();

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
    void onActionTimeEdited(const QString &goTime);
    void onActionCurrentFinished();

private:
    void resetPosition();
    KNMusicHeaderAlbumArt *m_albumArt;
    QLabel *m_title, *m_artist;
    KNPlayerProgress *m_progress;
    QPalette m_textPalette;
    KNMusicPlayer *m_player;
    KNMusicPlayerControl *m_playerControl;
    KNLabelEditor *m_time;
    QPropertyAnimation *m_mouseIn, *m_mouseOut;
    bool m_sliderPressed=false;
    KNMusicVisualEffect *m_visualEffect;
};

#endif // KNMUSICHEADERPLAYER_H
