#include <QLabel>
#include <QBoxLayout>

#include "knmusicplaylistsongs.h"

#include "knmusicplaylistdisplay.h"

KNMusicPlaylistDisplay::KNMusicPlaylistDisplay(QWidget *parent) :
    QWidget(parent)
{
    //Get the latest translation.
    retranslate();

    //Set properties.
    setAutoFillBackground(true);
    setContentsMargins(0,0,0,0);

    //Set layout.
    m_layout=new QBoxLayout(QBoxLayout::TopToBottom, this);
    m_layout->setContentsMargins(0,0,0,0);
    m_layout->setSpacing(0);
    setLayout(m_layout);

    //Set palette.
    QPalette pal=palette();
    pal.setColor(QPalette::Base, QColor(0x20, 0x20, 0x20));
    pal.setColor(QPalette::Window, QColor(0x20, 0x20, 0x20));
    pal.setColor(QPalette::Button, QColor(0x20, 0x20, 0x20));
    pal.setColor(QPalette::Text, QColor(0xff, 0xff, 0xff));
    pal.setColor(QPalette::Highlight, QColor(0x60, 0x60, 0x60));
    pal.setColor(QPalette::HighlightedText, QColor(0xf7, 0xcf, 0x3d));
    setPalette(pal);

    //Add captions.
    m_layout->addSpacing(20);
    m_playlistName=new QLabel(this);
    m_playlistName->setContentsMargins(20,0,0,0);
    QFont playlistFont=font();
    playlistFont.setPixelSize(playlistFont.pixelSize()+(playlistFont.pixelSize()>>1));
    playlistFont.setBold(true);
    m_playlistName->setFont(playlistFont);
    pal=m_playlistName->palette();
    pal.setColor(QPalette::WindowText, QColor(0xff, 0xff, 0xff));
    m_playlistName->setPalette(pal);
    m_layout->addWidget(m_playlistName);

    m_playlistSongCount=new QLabel(this);
    m_playlistSongCount->setContentsMargins(20,0,0,0);
    m_playlistSongCount->setPalette(pal);
    m_layout->addWidget(m_playlistSongCount);

    m_layout->addSpacing(20);
}

void KNMusicPlaylistDisplay::setPlaylistName(const QString &name)
{
    m_playlistName->setText(name);
}

void KNMusicPlaylistDisplay::setSongCount(const int &songCount)
{
    m_playlistSongCount->setText(songCount==1?
                                     m_songCaption:
                                     m_songsCaption.arg(QString::number(songCount)));
}

void KNMusicPlaylistDisplay::setSongsView(KNMusicPlaylistSongs *songsView)
{
    m_songsView=songsView;
    m_layout->addWidget(m_songsView);
}

void KNMusicPlaylistDisplay::retranslate()
{
    m_songCaption=tr("1 song.");
    m_songsCaption=tr("%1 songs.");
}

void KNMusicPlaylistDisplay::retranslateAndSet()
{
    ;
}
