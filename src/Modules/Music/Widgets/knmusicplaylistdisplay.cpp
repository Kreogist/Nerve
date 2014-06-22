#include <QLabel>
#include <QBoxLayout>

#include "knmusicplaylistsongs.h"

#include "knmusicplaylistdisplay.h"

KNMusicPlaylistDisplay::KNMusicPlaylistDisplay(QWidget *parent) :
    QWidget(parent)
{
    m_layout=new QBoxLayout(QBoxLayout::TopToBottom, this);
    setLayout(m_layout);

    m_playlistName=new QLabel(this);
    m_layout->addWidget(m_playlistName);
}

void KNMusicPlaylistDisplay::setPlaylistName(const QString &name)
{
    m_playlistName->setText(name);
}

void KNMusicPlaylistDisplay::setSongsView(KNMusicPlaylistSongs *songsView)
{
    m_songsView=songsView;
    m_layout->addWidget(m_songsView);
}
