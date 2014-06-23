#include <QBoxLayout>

#include <QDebug>

#include "knmusicplaylistlistview.h"
#include "knmusicplaylistlisteditor.h"
#include "knmusicplaylistdisplay.h"
#include "knmusicplaylistsongs.h"
#include "../Libraries/knmusicplaylistmanager.h"

#include "knmusicplaylistview.h"

KNMusicPlaylistView::KNMusicPlaylistView(QWidget *parent) :
    QSplitter(parent)
{
    //Set properties.
    setContentsMargins(0,0,0,0);
    setChildrenCollapsible(false);
    setOpaqueResize(false);
    setHandleWidth(0);

    //Set list widget.
    QWidget *playlistList=new QWidget(this);
    playlistList->setContentsMargins(0,0,0,0);
    QBoxLayout *layout=new QBoxLayout(QBoxLayout::TopToBottom, this);
    layout->setContentsMargins(0,0,0,0);
    layout->setSpacing(0);
    playlistList->setLayout(layout);
    m_playlistListView=new KNMusicPlaylistListview(this);
    connect(m_playlistListView, SIGNAL(activated(QModelIndex)),
            this, SLOT(onActionShowPlaylist(QModelIndex)));
    layout->addWidget(m_playlistListView, 1);
    m_playlistListEditor=new KNMusicPlaylistListEditor(this);
    layout->addWidget(m_playlistListEditor);
    addWidget(playlistList);

    //Set displayer.
    m_displayer=new KNMusicPlaylistDisplay(this);
    addWidget(m_displayer);
    m_songsView=new KNMusicPlaylistSongs(this);
    connect(m_songsView, &KNMusicPlaylistSongs::requireOpenUrl,
            this, &KNMusicPlaylistView::onActionOpenUrl);
    m_displayer->setSongsView(m_songsView);

    setCollapsible(1, false);
    setStretchFactor(1, 1);
}

void KNMusicPlaylistView::setManager(KNMusicPlaylistManager *manager)
{
    m_manager=manager;
    m_playlistListView->setModel(m_manager->playlistModel());
    connect(m_playlistListView->selectionModel(), &QItemSelectionModel::currentChanged,
            this, &KNMusicPlaylistView::onActionShowPlaylist);
}

void KNMusicPlaylistView::onActionShowPlaylist(const QModelIndex &index)
{
    m_displayer->setPlaylistName(m_manager->playlistName(index));
    m_songsView->setModel(m_manager->playlistDataModel(index));
    m_currentPath=m_manager->setModelPlaylist(index.row());
}

void KNMusicPlaylistView::onActionOpenUrl(const QModelIndex &index)
{
    m_manager->setPlaylist(m_currentPath);
    emit requirePlayMusic(m_manager->filePathFromIndex(index));
}
