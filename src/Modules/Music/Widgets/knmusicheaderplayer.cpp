#include <QBoxLayout>
#include <QResizeEvent>
#include <QPropertyAnimation>

#include <QDebug>

#include "knmusicplayercontrol.h"

#include "../Libraries/knmusicplayer.h"
#include "../../Base/knplayerprogress.h"
#include "../../Base/knlabeleditor.h"

#include "knmusicheaderplayer.h"

KNMusicHeaderAlbumArt::KNMusicHeaderAlbumArt(QWidget *parent) :
    QLabel(parent)
{
    ;
}

void KNMusicHeaderAlbumArt::mousePressEvent(QMouseEvent *event)
{
    QLabel::mousePressEvent(event);
    m_isPressed=true;
}

void KNMusicHeaderAlbumArt::mouseReleaseEvent(QMouseEvent *event)
{
    QLabel::mouseReleaseEvent(event);
    if(m_isPressed && rect().contains(event->pos()))
    {
        m_isPressed=false;
        if(m_isPlayerShown)
        {
            m_isPlayerShown=false;
            emit requireHideMusicPlayer();
        }
        else
        {
            m_isPlayerShown=true;
            emit requireShowMusicPlayer();
        }
    }
}

KNMusicHeaderPlayer::KNMusicHeaderPlayer(QWidget *parent) :
    QWidget(parent)
{

    QBoxLayout *albumArtLayout=new QBoxLayout(QBoxLayout::LeftToRight,
                                                this);
    albumArtLayout->setContentsMargins(0,0,0,0);
    setLayout(albumArtLayout);

    m_albumArt=new KNMusicHeaderAlbumArt(this);
    m_albumArt->setFixedSize(50,50);
    m_albumArt->setScaledContents(true);
    connect(m_albumArt, &KNMusicHeaderAlbumArt::requireShowMusicPlayer,
            this, &KNMusicHeaderPlayer::requireShowMusicPlayer);
    connect(m_albumArt, &KNMusicHeaderAlbumArt::requireHideMusicPlayer,
            this, &KNMusicHeaderPlayer::requireHideMusicPlayer);
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

    QBoxLayout *durationLayout=new QBoxLayout(QBoxLayout::LeftToRight,
                                              detailsArtLayout->widget());
    m_progress=new KNPlayerProgress(this);
    m_progress->setFixedWidth(200);
    connect(m_progress, &KNPlayerProgress::sliderPressed,
            [=]{m_sliderPressed=true;});
    connect(m_progress, &KNPlayerProgress::sliderReleased,
            [=]{
                    m_sliderPressed=false;
                    m_player->setPosition(m_progress->value());
               });
    durationLayout->addWidget(m_progress);
    m_time=new KNLabelEditor(this);
    m_time->setPalette(m_textPalette);
    connect(m_time, SIGNAL(textEdited(QString)),
            this, SLOT(onActionTimeEdited(QString)));
    durationLayout->addWidget(m_time);
    detailsArtLayout->addLayout(durationLayout);
    albumArtLayout->addLayout(detailsArtLayout);

    m_playerControl=new KNMusicPlayerControl(this);
    m_playerControl->hide();

    m_mouseIn=new QPropertyAnimation(m_playerControl, "pos", this);
    m_mouseIn->setEasingCurve(QEasingCurve::OutCubic);
    connect(m_mouseIn, SIGNAL(valueChanged(QVariant)),
            this, SLOT(onActionMouseInOut(QVariant)));

    m_mouseOut=new QPropertyAnimation(m_playerControl, "pos", this);
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
    connect(m_player, &KNMusicPlayer::finished,
            this, &KNMusicHeaderPlayer::onActionCurrentFinished);
    m_playerControl->setVolume(m_player->volume());
    connect(m_playerControl, &KNMusicPlayerControl::requirePlay,
            m_player, &KNMusicPlayer::play);
    connect(m_playerControl, &KNMusicPlayerControl::requirePause,
            m_player, &KNMusicPlayer::pause);
    connect(m_playerControl, &KNMusicPlayerControl::requireChangeVolume,
            m_player, &KNMusicPlayer::setVolume);
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
    m_mouseIn->setStartValue(m_playerControl->pos());
    m_mouseIn->setEndValue(QPoint(m_progress->x(),
                                  0));
    m_playerControl->show();
    m_mouseIn->start();
}

void KNMusicHeaderPlayer::leaveEvent(QEvent *event)
{
    QWidget::leaveEvent(event);
    m_mouseIn->stop();
    m_mouseOut->setStartValue(m_playerControl->pos());
    m_mouseOut->setEndValue(QPoint(m_progress->x(),
                                   -m_progress->y()));
    m_mouseOut->start();
}

void KNMusicHeaderPlayer::resizeEvent(QResizeEvent *event)
{
    QWidget::resizeEvent(event);
    m_playerControl->resize(width()-m_albumArt->width(),
                            m_progress->y());
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
    QString second=QString::number(position%60);
    m_time->setText(QString::number(position/60)+":"+
                    (second.length()==1?"0"+second:second));
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
    QPoint controlPosition=controlPos.toPoint();
    m_textPalette.setColor(QPalette::WindowText, QColor(255,255,255,-controlPosition.y()*6));
    m_title->setPalette(m_textPalette);
    m_artist->setPalette(m_textPalette);
}

void KNMusicHeaderPlayer::onActionTimeEdited(const QString &goTime)
{
    //Find the ':' char
    int colonPos=goTime.indexOf(':');
    // if no colon, treat it as a number of seconds.
    if(colonPos==-1)
    {
        m_player->setPosition(goTime.toInt());
        return;
    }
    //If we find colon, set the position to the new time.
    m_player->setPosition(goTime.left(colonPos).toInt()*60+
                          goTime.mid(colonPos+1).toInt());
}

void KNMusicHeaderPlayer::onActionCurrentFinished()
{
    //Should judge the mode of the player.
    ;
}

void KNMusicHeaderPlayer::resetPosition()
{
    m_progress->setValue(0);
}
