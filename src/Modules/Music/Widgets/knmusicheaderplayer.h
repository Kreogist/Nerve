#ifndef KNMUSICHEADERPLAYER_H
#define KNMUSICHEADERPLAYER_H

#include <QWidget>
#include <QLabel>

class QMouseEvent;
class QBoxLayout;
class QPropertyAnimation;
class KNPlayerProgress;
class KNLabelEditor;
class KNMusicBackend;
class KNMusicLoop;
class KNMusicPlayerControl;
class KNMusicAlbumArt;
class KNMusicHeaderPlayer : public QWidget
{
    Q_OBJECT
public:
    explicit KNMusicHeaderPlayer(QWidget *parent = 0);
    ~KNMusicHeaderPlayer();
    QPixmap albumArt() const;
    QWidget *progressBar();
    QString title() const;
    QString artistAlbum() const;
    void setAlbumArt(const QPixmap &albumArt);
    void setTitle(const QString &string);
    void setArtist(const QString &string);
    void setAlbum(const QString &string);
    float position() const;
    void putBackProgressBar();
    void setMusicBackend(KNMusicBackend *player);
    void playFile(const QString &filePath);
    void play();
    void stop();
    void syncData();

signals:
    void requireShowInCurrent();
    void requireShowMusicPlayer();
    void requireHideMusicPlayer();
    void requireChangeLoop(const int &loop);
    void requireSyncData();
    void requireNext();
    void requirePrev();
    void finished();

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

private:
    void resetPosition();
    KNMusicLoop *m_loopControl;
    KNMusicAlbumArt *m_albumArt;
    QLabel *m_title, *m_artistAlbum;
    QString m_artist, m_album;
    KNPlayerProgress *m_progress;
    QPalette m_textPalette;
    KNMusicBackend *m_player;
    KNMusicPlayerControl *m_playerControl;
    KNLabelEditor *m_time;
    QPropertyAnimation *m_mouseIn, *m_mouseOut;
    QPixmap m_albumArtImage;
    QWidget *m_progresBar;
    QBoxLayout *m_detailsArtLayout;
    bool m_sliderPressed=false, m_animate=true;
};

#endif // KNMUSICHEADERPLAYER_H
