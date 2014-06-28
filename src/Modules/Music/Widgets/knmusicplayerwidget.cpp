#include <QLabel>
#include <QPropertyAnimation>
#include <QBoxLayout>
#include <QResizeEvent>

#include <QDebug>

#include "../../Base/knopacitybutton.h"
#include "../Libraries/knmusiclibrarymodel.h"
#include "../Libraries/knmusicplayerbackend.h"
#include "knmusicvisualeffect.h"
#include "knmusicheaderplayer.h"
#include "knmusicplayerwidget.h"

KNMusicPlayerWidget::KNMusicPlayerWidget(QWidget *parent) :
    QWidget(parent)
{
    setAutoFillBackground(true);
    retranslate();

    QPalette pal=palette();
    pal.setColor(QPalette::Window, QColor(0,0,0,240));
    pal.setColor(QPalette::WindowText, QColor(255,255,255));
    setPalette(pal);

    m_mainLayout=new QBoxLayout(QBoxLayout::TopToBottom,
                                this);
    m_mainLayout->setContentsMargins(0,0,0,0);
    m_mainLayout->setSpacing(0);
    setLayout(m_mainLayout);
    m_visualEffect=new KNMusicVisualEffect(this);
    m_mainLayout->addWidget(m_visualEffect, 1);
    m_artworkLayout=new QBoxLayout(QBoxLayout::LeftToRight,
                                             m_mainLayout->widget());
    m_albumArtWork=new QLabel(this);
    m_albumArtWork->setScaledContents(true);
    m_artworkLayout->addWidget(m_albumArtWork, 0, Qt::AlignBottom | Qt::AlignRight);
    m_detailsLayout=new QBoxLayout(QBoxLayout::TopToBottom,
                                   m_mainLayout->widget());
    m_detailsLayout->setSpacing(0);
    m_detailsLayout->addStretch();
    m_controlsLayout=new QBoxLayout(QBoxLayout::LeftToRight,
                                    m_detailsLayout->widget());
    m_equalizerControl=new KNOpacityButton(this);
    m_equalizerControl->setPixmap(QPixmap(":/Music/Resources/Music/player/equalizer.png"));
    connect(m_equalizerControl, &KNOpacityButton::clicked,
            this, &KNMusicPlayerWidget::onActionShowEqualizer);
    m_controlsLayout->addWidget(m_equalizerControl);
    m_playlistControl=new KNOpacityButton(this);
    m_playlistControl->setPixmap(QPixmap(":/Music/Resources/Music/player/playlist.png"));
    m_controlsLayout->addWidget(m_playlistControl);
    m_controlsLayout->addStretch();
    m_detailsLayout->addLayout(m_controlsLayout);
    m_nowPlaying=new QLabel(this);
    QPalette textPalette=m_nowPlaying->palette();
    textPalette.setColor(QPalette::WindowText, QColor(0xf1, 0xf1, 0xf1));
    m_nowPlaying->setPalette(textPalette);
    m_nowPlaying->setText(m_nowPlayingText);
    m_nowPlayingFont=m_nowPlaying->font();
    m_detailsLayout->addWidget(m_nowPlaying);
    m_artistAlbumName=new QLabel(this);
    textPalette.setColor(QPalette::WindowText, QColor(0x99, 0x99, 0x99));
    m_artistAlbumName->setPalette(textPalette);
    m_detailsLayout->addWidget(m_artistAlbumName);
    m_title=new QLabel(this);
    m_titleFont=m_title->font();
    textPalette.setColor(QPalette::WindowText, QColor(0xff, 0xa5, 0x00));
    m_title->setPalette(textPalette);
    m_detailsLayout->addWidget(m_title);
    m_artworkLayout->addLayout(m_detailsLayout, 1);
    m_mainLayout->addLayout(m_artworkLayout);

    m_equalizerShow=new QPropertyAnimation(this);
    m_equalizerShow->setPropertyName("size");
    m_equalizerShow->setEasingCurve(QEasingCurve::OutCubic);

    m_equalizerHide=new QPropertyAnimation(this);
    m_equalizerHide->setPropertyName("size");
    m_equalizerHide->setEasingCurve(QEasingCurve::OutCubic);
}

void KNMusicPlayerWidget::setEqualizer(QWidget *equalizer)
{
    m_equalizer=equalizer;
    m_equalizer->setParent(this);
    m_equalizer->resize(QSize(height()-m_visualEffect->width(),
                              0));
//    m_mainLayout
    m_equalizerShow->setTargetObject(m_equalizer);
    m_equalizerHide->setTargetObject(m_equalizer);
}

void KNMusicPlayerWidget::setBackend(KNMusicPlayerBackend *backend)
{
    m_backend=backend;
    connect(m_backend, &KNMusicPlayerBackend::positionChanged,
            this, &KNMusicPlayerWidget::onActionPositionChanged);
}

void KNMusicPlayerWidget::setPlayListModel(QAbstractItemModel *model)
{
    ;
}

void KNMusicPlayerWidget::setHeaderPlayer(KNMusicHeaderPlayer *headerPlayer)
{
    m_headerPlayer=headerPlayer;
    connect(m_headerPlayer, &KNMusicHeaderPlayer::requireSyncData,
            this, &KNMusicPlayerWidget::syncData);
}

void KNMusicPlayerWidget::setArtwork(const QPixmap &artwork)
{
    m_albumArtWork->setPixmap(artwork);
}

void KNMusicPlayerWidget::retranslate()
{
    m_nowPlayingText=tr("Now Playing");
}

void KNMusicPlayerWidget::retranslateAndSet()
{
    ;
}

void KNMusicPlayerWidget::syncData()
{
    m_albumArtWork->setPixmap(m_headerPlayer->albumArt());
    m_title->setText(m_headerPlayer->title());
    m_artistAlbumName->setText(m_headerPlayer->artistAlbum());
}

void KNMusicPlayerWidget::resizeEvent(QResizeEvent *event)
{
    QWidget::resizeEvent(event);
    QSize sizeChangeParam=event->size();
    int sizeParam=qMin(sizeChangeParam.width(), sizeChangeParam.height());
    float sizeParamF=(float)sizeParam,
          artWorkSizeF=sizeParamF*0.32;
    int artWorkSize=(int)(artWorkSizeF),
        normalTextSize=(int)(artWorkSizeF*0.08),
        bottomMargin=(int)(artWorkSizeF*0.13),
        titleTextSize=bottomMargin;
    if(normalTextSize<15)
        normalTextSize=15;
    if(titleTextSize<30)
        titleTextSize=30;
    m_artworkLayout->setContentsMargins((int)(artWorkSizeF*0.3),
                                        0,
                                        0,
                                        bottomMargin);
    m_artworkLayout->setSpacing((int)(artWorkSizeF*0.23));
    m_detailsLayout->setSpacing((int)(artWorkSizeF*0.03));
    m_albumArtWork->setFixedSize(artWorkSize,artWorkSize);
    m_artistAlbumName->setContentsMargins(bottomMargin,0,0,0);
    m_title->setContentsMargins(bottomMargin,0,0,0);
    m_nowPlayingFont.setPixelSize(normalTextSize);
    m_nowPlaying->setFont(m_nowPlayingFont);
    m_artistAlbumName->setFont(m_nowPlayingFont);
    m_titleFont.setPixelSize(titleTextSize);
    m_title->setFont(m_titleFont);
    m_mainLayout->setSpacing(bottomMargin);
    int equalizerHeight=(bottomMargin<<1)+artWorkSize;
    m_equalizer->setGeometry(0,
                             sizeChangeParam.height()-equalizerHeight,
                             sizeChangeParam.width(),
                             m_equalizer->height()==0?0:equalizerHeight);
    m_equalizerControl->setFixedSize(normalTextSize, normalTextSize);
    m_playlistControl->setFixedSize(normalTextSize, normalTextSize);
}

void KNMusicPlayerWidget::onActionPositionChanged(const int &position)
{
    Q_UNUSED(position);
    m_backend->getGraphicData(m_visualEffect->fftData());
    m_visualEffect->update();
}

void KNMusicPlayerWidget::onActionShowEqualizer()
{
    m_equalizerHide->stop();
    m_equalizerShow->setStartValue(m_equalizer->size());
    m_equalizerShow->setEndValue(QSize(m_visualEffect->width(),
                                       height()-m_visualEffect->height()));
    m_equalizerShow->start();
}

void KNMusicPlayerWidget::onActionHideEqualizer()
{
    m_equalizerShow->stop();
    m_equalizerHide->setStartValue(m_equalizer->size());
    m_equalizerHide->setEndValue(QSize(m_visualEffect->width(),
                                       0));
    m_equalizerHide->start();
}
