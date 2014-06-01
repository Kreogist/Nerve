#include <QPropertyAnimation>
#include <QMouseEvent>
#include <QGraphicsOpacityEffect>
#include <QBoxLayout>

#include <QDebug>

#include "knmusicplayercontrol.h"

KNMusicPlayerControlButton::KNMusicPlayerControlButton(QWidget *parent) :
    QLabel(parent)
{
    setScaledContents(true);
    m_effect=new QGraphicsOpacityEffect(this);
    m_effect->setOpacity(0.3);
    setGraphicsEffect(m_effect);
    m_mouseDown=new QPropertyAnimation(m_effect, "opacity", this);
    m_mouseDown->setDuration(100);
    m_mouseDown->setEasingCurve(QEasingCurve::OutCubic);
    m_mouseDown->setEndValue(1.0);

    m_mouseUp=new QPropertyAnimation(m_effect, "opacity", this);
    m_mouseUp->setDuration(100);
    m_mouseUp->setEasingCurve(QEasingCurve::OutCubic);
    m_mouseUp->setEndValue(0.8);

    m_mouseIn=new QPropertyAnimation(m_effect, "opacity", this);
    m_mouseIn->setDuration(100);
    m_mouseIn->setEasingCurve(QEasingCurve::OutCubic);
    m_mouseIn->setEndValue(0.8);

    m_mouseOut=new QPropertyAnimation(m_effect, "opacity", this);
    m_mouseOut->setDuration(100);
    m_mouseOut->setEasingCurve(QEasingCurve::OutCubic);
    m_mouseOut->setEndValue(0.3);
}

void KNMusicPlayerControlButton::enterEvent(QEvent *event)
{
    QWidget::enterEvent(event);
    m_mouseOut->stop();
    m_mouseIn->setStartValue(m_effect->opacity());
    m_mouseIn->start();
}

void KNMusicPlayerControlButton::leaveEvent(QEvent *event)
{
    QWidget::leaveEvent(event);
    m_mouseIn->stop();
    m_mouseOut->setStartValue(m_effect->opacity());
    m_mouseOut->start();
}

void KNMusicPlayerControlButton::mousePressEvent(QMouseEvent *event)
{
    QLabel::mousePressEvent(event);
    emit mousePressed();
    m_mouseUp->stop();
    m_mouseDown->setStartValue(m_effect->opacity());
    m_mouseDown->start();
    m_isPressed=true;
}

void KNMusicPlayerControlButton::mouseReleaseEvent(QMouseEvent *event)
{
    QLabel::mouseReleaseEvent(event);
    emit mouseReleased();
    m_mouseDown->stop();
    if(m_isPressed && rect().contains(event->pos()))
    {
        m_mouseUp->setStartValue(m_effect->opacity());
        m_mouseUp->start();
        emit clicked();
    }
    m_isPressed=false;
}

KNMusicPlayerControl::KNMusicPlayerControl(QWidget *parent) :
    QWidget(parent)
{
    m_playButton=QPixmap(":/Music/Resources/Music/player/play.png");
    m_pauseButton=QPixmap(":/Music/Resources/Music/player/pause.png");
    setContentsMargins(0,0,0,0);
    m_mainLayout=new QBoxLayout(QBoxLayout::LeftToRight, this);
    m_mainLayout->setContentsMargins(0,0,0,0);
    m_mainLayout->setSpacing(0);
    setLayout(m_mainLayout);
    m_mainLayout->addStretch();
    m_prev=new KNMusicPlayerControlButton(this);
    m_prev->setFixedSize(32,32);
    m_prev->setPixmap(QPixmap(":/Music/Resources/Music/player/previous.png"));
    m_mainLayout->addWidget(m_prev);
    m_playnpause=new KNMusicPlayerControlButton(this);
    m_playnpause->setFixedSize(32,32);
    showPlaying(true);
    m_mainLayout->addWidget(m_playnpause);
    m_next=new KNMusicPlayerControlButton(this);
    m_next->setFixedSize(32,32);
    m_next->setPixmap(QPixmap(":/Music/Resources/Music/player/next.png"));
    m_mainLayout->addWidget(m_next);
    m_mainLayout->addStretch();
    connect(m_prev, &KNMusicPlayerControlButton::clicked,
            this, &KNMusicPlayerControl::requirePrev);
    connect(m_playnpause, &KNMusicPlayerControlButton::clicked,
            this, &KNMusicPlayerControl::onActionPlaynPauseClick);
    connect(m_next, &KNMusicPlayerControlButton::clicked,
            this, &KNMusicPlayerControl::requireNext);
}

void KNMusicPlayerControl::showPlaying(bool showned)
{
    m_isShownPlay=showned;
    m_playnpause->setPixmap(m_isShownPlay?m_playButton:m_pauseButton);
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
