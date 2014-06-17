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
    layout->addWidget(m_playlistListView, 1);
    m_playlistListEditor=new KNMusicPlaylistListEditor(this);
    layout->addWidget(m_playlistListEditor);
    addWidget(playlistList);

    //Set displayer.
    m_displayer=new KNMusicPlaylistDisplay(this);
    addWidget(m_displayer);
    m_songsView=new KNMusicPlaylistSongs(this);
    m_displayer->setSongsView(m_songsView);

    setCollapsible(1, false);
    setStretchFactor(1, 1);
}

void KNMusicPlaylistView::setManager(KNMusicPlaylistManager *manager)
{
    m_manager=manager;
    m_playlistListView->setModel(m_manager->playlistModel());
    m_songsView->setModel(m_manager->playlistDataModel());
    connect(m_playlistListView->selectionModel(), &QItemSelectionModel::currentChanged,
            this, &KNMusicPlaylistView::onActionShowPlaylist);
}

void KNMusicPlaylistView::onActionShowPlaylist(const QModelIndex &index)
{
    m_manager->setPlaylist(index.row());
}
