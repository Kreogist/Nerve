#include <QBoxLayout>
#include <QResizeEvent>
#include <QPropertyAnimation>

#include <QDebug>

#include "knmusicplayercontrol.h"
#include "knmusicloop.h"
#include "knmusicalbumart.h"
#include "../../Public/Base/knmusicbackend.h"
#include "../../Base/knplayerprogress.h"
#include "../../Base/knlabeleditor.h"
#include "../../Base/knscrolllabel.h"

#include "knmusicheaderplayer.h"

KNMusicHeaderPlayer::KNMusicHeaderPlayer(QWidget *parent) :
    QWidget(parent)
{
    //Set properties.
    setContentsMargins(0,0,0,0);
    setFixedWidth(325);
    setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::MinimumExpanding);

    //Initial the layouts and widgets.
    QBoxLayout *albumArtLayout=new QBoxLayout(QBoxLayout::LeftToRight,
                                                this);
    albumArtLayout->setContentsMargins(0,0,0,0);
    albumArtLayout->setSpacing(10);
    setLayout(albumArtLayout);

    m_albumArt=new KNMusicAlbumArt(this);
    m_albumArt->setFixedSize(50,50);
    m_albumArt->setScaledContents(true);
    connect(m_albumArt, &KNMusicAlbumArt::requireShowMusicPlayer,
            [=]{
                    emit requireShowMusicPlayer();
                    m_animate=false;
               });
    connect(m_albumArt, &KNMusicAlbumArt::requireHideMusicPlayer,
            [=]{
                    emit requireHideMusicPlayer();
                    m_animate=true;
               });
    albumArtLayout->addWidget(m_albumArt);

    m_detailsArtLayout=new QBoxLayout(QBoxLayout::TopToBottom,
                                      albumArtLayout->widget());
    m_detailsArtLayout->setContentsMargins(0,10,0,10);
    m_detailsArtLayout->setSpacing(0);
    m_title=new KNScrollLabel(this);
    m_textPalette=m_title->palette();
    m_textPalette.setColor(QPalette::WindowText, QColor(255,255,255));
    m_title->setPalette(m_textPalette);
    m_detailsArtLayout->addWidget(m_title);

    m_artistAlbum=new KNScrollLabel(this);
    m_artistAlbum->setPalette(m_textPalette);
    m_detailsArtLayout->addWidget(m_artistAlbum);

    m_progresBar=new QWidget;
    m_progresBar->setContentsMargins(0,0,0,0);
    QBoxLayout *durationLayout=new QBoxLayout(QBoxLayout::LeftToRight,
                                              m_detailsArtLayout->widget());
    durationLayout->setContentsMargins(0,0,0,0);
    durationLayout->setSpacing(3);
    m_progresBar->setLayout(durationLayout);
    //Add loop controller.
    m_loopControl=new KNMusicLoop(this);
    connect(m_loopControl, &KNMusicLoop::requireChangeLoopType,
            this, &KNMusicHeaderPlayer::requireChangeLoop);
    durationLayout->addWidget(m_loopControl, 0, Qt::AlignVCenter);
    //Add duration display.
    m_time=new KNLabelEditor(this);
    m_time->setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
    m_time->setPalette(m_textPalette);
    connect(m_time, &KNLabelEditor::textEdited,
            this, &KNMusicHeaderPlayer::onActionTimeEdited);
    durationLayout->addWidget(m_time, 0, Qt::AlignVCenter);
    //Add playing progress slider displayer
    m_progress=new KNPlayerProgress(this);
    m_progress->setMinimumWidth(200);
    connect(m_progress, &KNPlayerProgress::valueChanged,
            [=](const int &position)
            {
                QString second=QString::number(position%60);
                m_time->setText(QString::number(position/60)+":"+
                                (second.length()==1?"0"+second:second));
            });
    connect(m_progress, &KNPlayerProgress::sliderPressed,
            [=]{m_sliderPressed=true;});
    connect(m_progress, &KNPlayerProgress::sliderReleased,
            [=]{
                    m_sliderPressed=false;
                    m_player->setPosition(m_progress->value());
               });
    durationLayout->addWidget(m_progress, 1, Qt::AlignVCenter);
    m_detailsArtLayout->addWidget(m_progresBar);
    albumArtLayout->addLayout(m_detailsArtLayout);

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

KNMusicHeaderPlayer::~KNMusicHeaderPlayer()
{
    m_progresBar->deleteLater();
}

QPixmap KNMusicHeaderPlayer::albumArt() const
{
    return m_albumArtImage;
}

QWidget *KNMusicHeaderPlayer::progressBar()
{
    return m_progresBar;
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

void KNMusicHeaderPlayer::putBackProgressBar()
{
    m_detailsArtLayout->addWidget(m_progresBar);
}

void KNMusicHeaderPlayer::setMusicBackend(KNMusicBackend *player)
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
        m_mouseIn->stop();
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
        m_mouseOut->stop();
        m_mouseOut->setStartValue(m_playerControl->pos());
        m_mouseOut->setEndValue(QPoint(m_albumArt->width(),
                                       -m_artistAlbum->geometry().bottom()));
        m_mouseOut->start();
    }
}

void KNMusicHeaderPlayer::resizeEvent(QResizeEvent *event)
{
    QWidget::resizeEvent(event);
    m_playerControl->resize(width()-m_albumArt->width(),
                            m_artistAlbum->geometry().bottom());
}

void KNMusicHeaderPlayer::onActionPositonChanged(const int &position)
{
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
    QPoint controlPosition=controlPos.toPoint();
    m_textPalette.setColor(QPalette::WindowText, QColor(255,255,255,-controlPosition.y()*5));
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
