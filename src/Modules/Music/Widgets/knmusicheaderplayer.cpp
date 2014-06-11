#include <QBoxLayout>
#include <QResizeEvent>
#include <QPropertyAnimation>

#include <QDebug>

#include "knmusicplayercontrol.h"
#include "knmusicloop.h"
#include "../Libraries/knmusicbackend.h"
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
    //Set properties.
    setContentsMargins(0,0,0,0);

    //Initial the layouts and widgets.
    QBoxLayout *albumArtLayout=new QBoxLayout(QBoxLayout::LeftToRight,
                                                this);
    albumArtLayout->setContentsMargins(0,0,0,0);
    setLayout(albumArtLayout);

    m_albumArt=new KNMusicHeaderAlbumArt(this);
    m_albumArt->setFixedSize(50,50);
    m_albumArt->setScaledContents(true);
    connect(m_albumArt, &KNMusicHeaderAlbumArt::requireShowMusicPlayer,
            [=]{
                    emit requireShowMusicPlayer();
                    m_animate=false;
               });
    connect(m_albumArt, &KNMusicHeaderAlbumArt::requireHideMusicPlayer,
            [=]{
                    emit requireHideMusicPlayer();
                    m_animate=true;
               });
    albumArtLayout->addWidget(m_albumArt);

    QBoxLayout *detailsArtLayout=new QBoxLayout(QBoxLayout::TopToBottom,
                                                albumArtLayout->widget());

    m_title=new QLabel(this);
    m_textPalette=m_title->palette();
    m_textPalette.setColor(QPalette::WindowText, QColor(255,255,255));
    m_title->setPalette(m_textPalette);
    detailsArtLayout->addWidget(m_title);

    m_artistAlbum=new QLabel(this);
    m_artistAlbum->setPalette(m_textPalette);
    detailsArtLayout->addWidget(m_artistAlbum);

    QBoxLayout *durationLayout=new QBoxLayout(QBoxLayout::LeftToRight,
                                              detailsArtLayout->widget());
    durationLayout->setContentsMargins(0,0,0,0);
    durationLayout->setSpacing(3);
    //Add loop controller.
    m_loopControl=new KNMusicLoop(this);
    connect(m_loopControl, &KNMusicLoop::requireChangeLoopType,
            this, &KNMusicHeaderPlayer::requireChangeLoop);
    durationLayout->addWidget(m_loopControl);
    //Add duration display.
    m_time=new KNLabelEditor(this);
    m_time->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
    m_time->setPalette(m_textPalette);
    connect(m_time, &KNLabelEditor::textEdited,
            this, &KNMusicHeaderPlayer::onActionTimeEdited);
    durationLayout->addWidget(m_time);
    //Add playing progress slider displayer
    m_progress=new KNPlayerProgress(this);
    m_progress->setMinimumWidth(200);
    connect(m_progress, &KNPlayerProgress::sliderPressed,
            [=]{m_sliderPressed=true;});
    connect(m_progress, &KNPlayerProgress::sliderReleased,
            [=]{
                    m_sliderPressed=false;
                    m_player->setPosition(m_progress->value());
               });
    durationLayout->addWidget(m_progress, 1);
    detailsArtLayout->addLayout(durationLayout);
    albumArtLayout->addLayout(detailsArtLayout);

    m_playerControl=new KNMusicPlayerControl(this);
    m_playerControl->hide();

    m_mouseIn=new QPropertyAnimation(m_playerControl, "pos", this);
    m_mouseIn->setEasingCurve(QEasingCurve::OutCubic);
    connect(m_mouseIn, &QPropertyAnimation::valueChanged,
            this, &KNMusicHeaderPlayer::onActionMouseInOut);

    m_mouseOut=new QPropertyAnimation(m_playerControl, "pos", this);
    m_mouseOut->setEasingCurve(QEasingCurve::OutCubic);
    connect(m_mouseOut, &QPropertyAnimation::finished,
            m_playerControl, &KNMusicPlayerControl::hide);
    connect(m_mouseOut, &QPropertyAnimation::valueChanged,
            this, &KNMusicHeaderPlayer::onActionMouseInOut);
}

QPixmap KNMusicHeaderPlayer::albumArt() const
{
    return m_albumArtImage;
}

void KNMusicHeaderPlayer::setAlbumArt(const QPixmap &albumArt)
{
    m_albumArtImage=albumArt;
    m_albumArt->setPixmap(albumArt);
}

QString KNMusicHeaderPlayer::title() const
{
    return m_title->text();
}

void KNMusicHeaderPlayer::setTitle(const QString &string)
{
    m_title->setText(string);
}

QString KNMusicHeaderPlayer::artistAlbum() const
{
    return m_artistAlbum->text();
}

void KNMusicHeaderPlayer::setArtist(const QString &string)
{
    m_artist=string;
    if(m_album.isEmpty())
    {
        m_artistAlbum->setText(m_artist);
        return;
    }
    m_artistAlbum->setText(m_artist + " - " + m_album);
}

void KNMusicHeaderPlayer::setAlbum(const QString &string)
{
    m_album=string;
    if(m_artist.isEmpty())
    {
        m_artistAlbum->setText(m_album);
    }
    m_artistAlbum->setText(m_artist + " - " + m_album);
}

float KNMusicHeaderPlayer::position() const
{
    return m_player->position();
}

void KNMusicHeaderPlayer::setBackend(KNMusicBackend *player)
{
    m_player=player;
    connect(m_player, &KNMusicBackend::positionChanged,
            this, &KNMusicHeaderPlayer::onActionPositonChanged);
    connect(m_player, &KNMusicBackend::durationChanged,
            this, &KNMusicHeaderPlayer::onActionDurationChanged);
    connect(m_player, &KNMusicBackend::finished,
            this, &KNMusicHeaderPlayer::finished);
    m_playerControl->setVolume(m_player->volume());
    connect(m_playerControl, &KNMusicPlayerControl::requireNext,
            this, &KNMusicHeaderPlayer::requireNext);
    connect(m_playerControl, &KNMusicPlayerControl::requirePrev,
            this, &KNMusicHeaderPlayer::requirePrev);
    connect(m_playerControl, &KNMusicPlayerControl::requirePlay,
            [=]
            {
                m_player->play();
            });
    connect(m_playerControl, &KNMusicPlayerControl::requirePause,
            [=]
            {
                m_player->pause();
            });
    connect(m_playerControl, &KNMusicPlayerControl::requireChangeVolume,
            m_player, &KNMusicBackend::setVolume);
}

void KNMusicHeaderPlayer::playFile(const QString &filePath)
{
    resetPosition();
    m_player->playFile(filePath);
    m_playerControl->showPlaying(false);
}

void KNMusicHeaderPlayer::play()
{
    m_player->play();
}

void KNMusicHeaderPlayer::stop()
{
    m_player->stop();
}

void KNMusicHeaderPlayer::syncData()
{
    emit requireSyncData();
}

void KNMusicHeaderPlayer::enterEvent(QEvent *event)
{
    QWidget::enterEvent(event);
    if(m_animate)
    {
        m_mouseOut->stop();
        m_mouseIn->setStartValue(m_playerControl->pos());
        m_mouseIn->setEndValue(QPoint(m_albumArt->width(),
                                      0));
        m_playerControl->show();
        m_mouseIn->start();
    }
}

void KNMusicHeaderPlayer::leaveEvent(QEvent *event)
{
    QWidget::leaveEvent(event);
    if(m_animate)
    {
        m_mouseIn->stop();
        m_mouseOut->setStartValue(m_playerControl->pos());
        m_mouseOut->setEndValue(QPoint(m_albumArt->width(),
                                       -m_progress->y()));
        m_mouseOut->start();
    }
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
    m_artistAlbum->setPalette(m_textPalette);
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

void KNMusicHeaderPlayer::resetPosition()
{
    m_progress->setValue(0);
}
