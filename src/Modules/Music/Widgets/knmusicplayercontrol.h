#ifndef KNMUSICPLAYERCONTROL_H
#define KNMUSICPLAYERCONTROL_H

#include <QWidget>
#include <QLabel>
#include <QPixmap>

class QBoxLayout;
class KNOpacityButton;
class KNMusicVolumeSlider;
class KNMusicPlayerControl : public QWidget
{
    Q_OBJECT
public:
    explicit KNMusicPlayerControl(QWidget *parent = 0);
    void showPlaying(bool showned);
    void setVolume(const float &value);

signals:
    void requirePlay();
    void requirePause();
    void requirePrev();
    void requireNext();
    void requireChangeVolume(const float &value);

public slots:

private slots:
    void onActionPlaynPauseClick();

private:
    KNOpacityButton *m_prev, *m_playnpause, *m_next;
    KNMusicVolumeSlider *m_volume;
    QBoxLayout *m_mainLayout;
    QPixmap m_playButton, m_pauseButton;
    bool m_isShownPlay;
};

#endif // KNMUSICPLAYERCONTROL_H
