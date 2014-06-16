#include <QBoxLayout>

#include <QDebug>

#include "../../Base/knopacitybutton.h"
#include "knmusicvolumeslider.h"

#include "knmusicplayercontrol.h"

KNMusicPlayerControl::KNMusicPlayerControl(QWidget *parent) :
    QWidget(parent)
{
    //Initial properties.
    setContentsMargins(0,0,0,0);

    //Initial pixmaps.
    m_playButton=QPixmap(":/Music/Resources/Music/player/play.png");
    m_pauseButton=QPixmap(":/Music/Resources/Music/player/pause.png");

    //Initial layout.
    m_mainLayout=new QBoxLayout(QBoxLayout::LeftToRight, this);
    m_mainLayout->setContentsMargins(5,10,0,0);
    m_mainLayout->setSpacing(0);
    setLayout(m_mainLayout);

    m_mainLayout->addStretch();
    m_prev=new KNOpacityButton(this);
    m_prev->setFixedSize(32,32);
    m_prev->setPixmap(QPixmap(":/Music/Resources/Music/player/previous.png"));
    m_mainLayout->addWidget(m_prev, 0, Qt::AlignVCenter);
    m_playnpause=new KNOpacityButton(this);
    m_playnpause->setFixedSize(32,32);
    showPlaying(true);
    m_mainLayout->addWidget(m_playnpause, 0, Qt::AlignVCenter);
    m_next=new KNOpacityButton(this);
    m_next->setFixedSize(32,32);
    m_next->setPixmap(QPixmap(":/Music/Resources/Music/player/next.png"));
    m_mainLayout->addWidget(m_next, 0, Qt::AlignVCenter);
    m_volume=new KNMusicVolumeSlider(this);
    m_volume->setRange(0, 10000);
    connect(m_volume, &KNMusicVolumeSlider::valueChanged,
            this, &KNMusicPlayerControl::requireChangeVolume);
    m_mainLayout->addWidget(m_volume, 1, Qt::AlignVCenter);
    connect(m_prev, &KNOpacityButton::clicked,
            this, &KNMusicPlayerControl::requirePrev);
    connect(m_playnpause, &KNOpacityButton::clicked,
            this, &KNMusicPlayerControl::onActionPlaynPauseClick);
    connect(m_next, &KNOpacityButton::clicked,
            this, &KNMusicPlayerControl::requireNext);
}

void KNMusicPlayerControl::showPlaying(bool showned)
{
    m_isShownPlay=showned;
    m_playnpause->setPixmap(m_isShownPlay?m_playButton:m_pauseButton);
}

void KNMusicPlayerControl::setVolume(const float &value)
{
    m_volume->setValue(value);
}

void KNMusicPlayerControl::onActionPlaynPauseClick()
{
    if(m_isShownPlay)
    {
        //Means is paused or stopped.
        showPlaying(false);
        emit requirePlay();
        return;
    }
    //Means is playing.
    showPlaying(true);
    emit requirePause();
}
