#ifndef KNMUSICPLAYERCONTROL_H
#define KNMUSICPLAYERCONTROL_H

#include <QWidget>
#include <QLabel>
#include <QPixmap>

class QBoxLayout;
class QPropertyAnimation;
class QGraphicsOpacityEffect;
class KNMusicPlayerControlButton : public QLabel
{
    Q_OBJECT
public:
    explicit KNMusicPlayerControlButton(QWidget *parent = 0);

signals:
    void clicked();
    void mousePressed();
    void mouseReleased();

protected:
    void enterEvent(QEvent *event);
    void leaveEvent(QEvent *event);
    void mousePressEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);

private:
    QGraphicsOpacityEffect *m_effect;
    QPropertyAnimation *m_mouseDown, *m_mouseUp, *m_mouseIn, *m_mouseOut;
    bool m_isPressed=false;
};

class KNMusicPlayerControl : public QWidget
{
    Q_OBJECT
public:
    explicit KNMusicPlayerControl(QWidget *parent = 0);
    void showPlaying(bool showned);

signals:
    void requirePlay();
    void requirePause();
    void requirePrev();
    void requireNext();

public slots:

private slots:
    void onActionPlaynPauseClick();

private:
    KNMusicPlayerControlButton *m_prev, *m_playnpause, *m_next;
    QBoxLayout *m_mainLayout;
    QPixmap m_playButton, m_pauseButton;
    bool m_isShownPlay;
};

#endif // KNMUSICPLAYERCONTROL_H
