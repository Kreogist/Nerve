#include <QBoxLayout>

#include <QDebug>

#include "knmusicplaylistlistview.h"
#include "knmusicplaylistlisteditor.h"
#include "knmusicplaylistdisplay.h"
#include "knmusicplaylistsongs.h"

#include "../Base/knmusicplaylistmanagerbase.h"
#include "../Base/knmusicplaylistlisteditorbase.h"

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
    m_listLayout=new QBoxLayout(QBoxLayout::TopToBottom, this);
    m_listLayout->setContentsMargins(0,0,0,0);
    m_listLayout->setSpacing(0);
    playlistList->setLayout(m_listLayout);
    m_playlistListView=new KNMusicPlaylistListview(this);
    connect(m_playlistListView, &KNMusicPlaylistListview::activated,
            this, &KNMusicPlaylistView::onActionShowPlaylist);
    m_listLayout->addWidget(m_playlistListView, 1);
    setListEditor(new KNMusicPlaylistListEditor);
    //Add list displayer.
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

void KNMusicPlaylistView::setManager(KNMusicPlaylistManagerBase *manager)
{
    m_manager=manager;
    m_playlistListView->setModel(m_manager->playlistModel());
    connect(m_playlistListView->selectionModel(), &QItemSelectionModel::currentChanged,
            this, &KNMusicPlaylistView::onActionShowPlaylist);
}

void KNMusicPlaylistView::setListEditor(KNMusicPlaylistListEditorBase *editor)
{
    m_playlistListEditor=editor;
    m_listLayout->addWidget(m_playlistListEditor);
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
    m_manager->setCurrentPlaylistPlaying(index);
    emit requirePlayMusic(m_manager->filePathFromIndex(index));
}
