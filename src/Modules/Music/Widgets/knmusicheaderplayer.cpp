#include <QBoxLayout>
#include <QLabel>
#include <QResizeEvent>
#include <QPropertyAnimation>

#include <QDebug>

#include "knmusicplayercontrol.h"
#include "knmusicvisualeffect.h"
#include "../Libraries/knmusicplayer.h"
#include "../../Base/knplayerprogress.h"

#include "knmusicheaderplayer.h"

KNMusicHeaderPlayer::KNMusicHeaderPlayer(QWidget *parent) :
    QWidget(parent)
{
//    m_visualEffect=new KNMusicVisualEffect;
//    m_visualEffect->resize(368, 70);
    QBoxLayout *albumArtLayout=new QBoxLayout(QBoxLayout::LeftToRight,
                                                this);
    albumArtLayout->setContentsMargins(0,0,0,0);
    setLayout(albumArtLayout);

    m_albumArt=new QLabel(this);
    m_albumArt->setFixedSize(50,50);
    m_albumArt->setScaledContents(true);
    albumArtLayout->addWidget(m_albumArt);

    QBoxLayout *detailsArtLayout=new QBoxLayout(QBoxLayout::TopToBottom,
                                                albumArtLayout->widget());

    m_title=new QLabel(this);
    m_textPalette=m_title->palette();
    m_textPalette.setColor(QPalette::WindowText, QColor(255,255,255));
    m_title->setPalette(m_textPalette);
    detailsArtLayout->addWidget(m_title);

    m_artist=new QLabel(this);
    m_artist->setPalette(m_textPalette);
    detailsArtLayout->addWidget(m_artist);

    m_progress=new KNPlayerProgress(this);
    m_progress->setFixedWidth(200);
    connect(m_progress, &KNPlayerProgress::sliderPressed,
            [=]{m_sliderPressed=true;});
    connect(m_progress, &KNPlayerProgress::sliderReleased,
            [=]{
                    m_sliderPressed=false;
                    m_player->setPosition(m_progress->value());
               });
    detailsArtLayout->addWidget(m_progress);
    albumArtLayout->addLayout(detailsArtLayout);

    m_playerControl=new KNMusicPlayerControl(this);
    m_playerControl->hide();

    m_mouseIn=new QPropertyAnimation(m_playerControl, "geometry", this);
    m_mouseIn->setEasingCurve(QEasingCurve::OutCubic);
    connect(m_mouseIn, SIGNAL(valueChanged(QVariant)),
            this, SLOT(onActionMouseInOut(QVariant)));

    m_mouseOut=new QPropertyAnimation(m_playerControl, "geometry", this);
    m_mouseOut->setEasingCurve(QEasingCurve::OutCubic);
    connect(m_mouseOut, SIGNAL(finished()),
            m_playerControl, SLOT(hide()));
    connect(m_mouseOut, SIGNAL(valueChanged(QVariant)),
            this, SLOT(onActionMouseInOut(QVariant)));
}

QWidget *KNMusicHeaderPlayer::visualEffect()
{
//    return m_visualEffect;
}

void KNMusicHeaderPlayer::setAlbumArt(const QPixmap &albumArt)
{
    m_albumArt->setPixmap(albumArt);
}

void KNMusicHeaderPlayer::setTitle(const QString &string)
{
    m_title->setText(string);
}

void KNMusicHeaderPlayer::setArtist(const QString &string)
{
    m_artist->setText(string);
}

void KNMusicHeaderPlayer::setPlayer(KNMusicPlayer *player)
{
    m_player=player;
    connect(m_player, &KNMusicPlayer::positionChanged,
            this, &KNMusicHeaderPlayer::onActionPositonChanged);
    connect(m_player, &KNMusicPlayer::durationChanged,
            this, &KNMusicHeaderPlayer::onActionDurationChanged);
    connect(m_playerControl, &KNMusicPlayerControl::requirePlay,
            m_player, &KNMusicPlayer::play);
    connect(m_playerControl, &KNMusicPlayerControl::requirePause,
            m_player, &KNMusicPlayer::pause);
}

void KNMusicHeaderPlayer::playFile(const QString &filePath)
{
    resetPosition();
    m_player->playFile(filePath);
    m_playerControl->showPlaying(false);
}

void KNMusicHeaderPlayer::enterEvent(QEvent *event)
{
    QWidget::enterEvent(event);
    m_mouseOut->stop();
    m_mouseIn->setStartValue(m_playerControl->geometry());
    m_mouseIn->setEndValue(QRect(m_progress->x(),
                                 0,
                                 m_progress->width(),
                                 m_progress->y()));
    m_playerControl->show();
    m_mouseIn->start();
}

void KNMusicHeaderPlayer::leaveEvent(QEvent *event)
{
    QWidget::leaveEvent(event);
    m_mouseIn->stop();
    m_mouseOut->setStartValue(m_playerControl->geometry());
    m_mouseOut->setEndValue(QRect(m_progress->x(),
                                  -m_progress->y(),
                                  m_progress->width(),
                                  m_progress->y()));
    m_mouseOut->start();
}

void KNMusicHeaderPlayer::resizeEvent(QResizeEvent *event)
{
    QWidget::resizeEvent(event);
    if(m_playerControl->isVisible())
    {
        m_playerControl->setGeometry(m_progress->x(),
                                     0,
                                     m_progress->width(),
                                     m_progress->y());
    }
}

void KNMusicHeaderPlayer::onActionPositonChanged(const int &position)
{
//    m_player->getGraphicData(m_visualEffect->fftData());
//    m_visualEffect->update();
    if(m_sliderPressed)
    {
        return;
    }
    m_progress->setValue(position);
}

void KNMusicHeaderPlayer::onActionDurationChanged(const int &duration)
{
    m_progress->setMaximum(duration);
}

void KNMusicHeaderPlayer::onActionReachEndOfMusic()
{
    m_progress->setValue(m_progress->maximum());
}

void KNMusicHeaderPlayer::onActionMouseInOut(const QVariant &controlPos)
{
    QRect controlPosition=controlPos.toRect();
    m_textPalette.setColor(QPalette::WindowText, QColor(255,255,255,-controlPosition.y()*6));
    m_title->setPalette(m_textPalette);
    m_artist->setPalette(m_textPalette);
}

void KNMusicHeaderPlayer::resetPosition()
{
    m_progress->setValue(0);
}
