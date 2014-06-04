#include <QLabel>
#include <QBoxLayout>
#include <QResizeEvent>

#include <QDebug>

#include "../Libraries/knmusicmodel.h"
#include "../Libraries/knmusicbackend.h"
#include "knmusicvisualeffect.h"
#include "knmusicheaderplayer.h"
#include "knmusicplayerwidget.h"

KNMusicPlayerWidget::KNMusicPlayerWidget(QWidget *parent) :
    QWidget(parent)
{
    setAutoFillBackground(true);
    retranslate();

    QPalette pal=palette();
    pal.setColor(QPalette::Window, QColor(0,0,0,200));
    pal.setColor(QPalette::WindowText, QColor(255,255,255));
    setPalette(pal);

    m_mainLayout=new QBoxLayout(QBoxLayout::TopToBottom, this);
    m_mainLayout->setContentsMargins(0,0,0,0);
    m_mainLayout->setSpacing(0);
    setLayout(m_mainLayout);

    m_playListLayout=new QBoxLayout(QBoxLayout::LeftToRight,
                                    m_mainLayout->widget());
    m_playListLayout->setContentsMargins(0,0,0,0);
    m_playListLayout->setSpacing(0);
    m_visualEffect=new KNMusicVisualEffect(this);
    m_mainLayout->addLayout(m_playListLayout);

    m_artworkLayout=new QBoxLayout(QBoxLayout::LeftToRight,
                                             m_mainLayout->widget());
    m_albumArtWork=new QLabel(this);
    m_albumArtWork->setScaledContents(true);
    m_artworkLayout->addWidget(m_albumArtWork, 0, Qt::AlignBottom | Qt::AlignRight);
    m_detailsLayout=new QBoxLayout(QBoxLayout::TopToBottom,
                                   m_mainLayout->widget());
    m_detailsLayout->setSpacing(0);
    m_detailsLayout->addStretch();
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
}

void KNMusicPlayerWidget::setEqualizer(QWidget *equalizer)
{
    m_equalizer=equalizer;
    m_equalizer->setParent(this);
    m_equalizer->hide();
}

void KNMusicPlayerWidget::setBackend(KNMusicBackend *backend)
{
    m_backend=backend;
    connect(m_backend, &KNMusicBackend::positionChanged,
            this, &KNMusicPlayerWidget::onActionPositionChanged);
}

void KNMusicPlayerWidget::setMusicModel(KNMusicModel *model)
{
    m_musicModel=model;
}

void KNMusicPlayerWidget::setArtwork(const QPixmap &artwork)
{
    m_albumArtWork->setPixmap(artwork);
}

void KNMusicPlayerWidget::retranslate()
{
    m_nowPlayingText=tr("Now Playing");
    m_noAlbumName=tr("No Album");
    m_noArtistName=tr("No Artist");
}

void KNMusicPlayerWidget::retranslateAndSet()
{
    ;
}

void KNMusicPlayerWidget::syncData(const QModelIndex &index)
{
    m_noAritst=true;m_noAlbum=true;
    if(index.isValid())
    {
        m_albumArtWork->setPixmap(QPixmap::fromImage(m_musicModel->artwork(index.row())));
        m_title->setText(m_musicModel->itemText(index.row(), KNMusicGlobal::Name));
        QString albumName=m_musicModel->itemText(index.row(), KNMusicGlobal::Album),
                artistName=m_musicModel->itemText(index.row(), KNMusicGlobal::Artist);
        m_noAritst=artistName.isEmpty();m_artistName=artistName;
        m_noAlbum=albumName.isEmpty();m_albumName=albumName;
        m_artistAlbumName->setText((m_noAritst?m_noArtistName:m_artistName) +
                                    " - " +
                                    (m_noAlbum?m_noAlbumName:m_albumName));
    }
}

void KNMusicPlayerWidget::resizeEvent(QResizeEvent *event)
{
    QWidget::resizeEvent(event);
    int sizeParam=qMin(width(), height());
    float sizeParamF=(float)sizeParam,
          artWorkSizeF=sizeParamF*0.32;
    int artWorkSize=(int)(artWorkSizeF),
        normalTextSize=(int)(artWorkSizeF*0.08),
        bottomMargin=(int)(artWorkSizeF*0.13);
    if(normalTextSize<10)
        normalTextSize=10;
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
    m_titleFont.setPixelSize((int)(artWorkSizeF*0.13));
    m_title->setFont(m_titleFont);
    m_visualEffect->resize(width(), height()-artWorkSize-bottomMargin);
}

void KNMusicPlayerWidget::onActionPositionChanged(const int &position)
{
    Q_UNUSED(position);
    m_backend->getGraphicData(m_visualEffect->fftData());
    m_visualEffect->update();
}
