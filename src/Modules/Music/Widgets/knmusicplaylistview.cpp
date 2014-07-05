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
    m_playlistListView=new KNMusicPlaylistListView(this);
    connect(m_playlistListView, &KNMusicPlaylistListView::activated,
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
    connect(m_manager, &KNMusicPlaylistManagerBase::requireUpdateItem,
            this, &KNMusicPlaylistView::onActionItemUpdate);
}

void KNMusicPlaylistView::setListEditor(KNMusicPlaylistListEditorBase *editor)
{
    m_playlistListEditor=editor;
    connect(m_playlistListEditor, &KNMusicPlaylistListEditorBase::requireAddPlaylist,
            this, &KNMusicPlaylistView::onActionAddPlaylist);
    connect(m_playlistListEditor, &KNMusicPlaylistListEditorBase::requireRemoveCurrentPlaylist,
            this, &KNMusicPlaylistView::onActionRemoveCurrentPlaylist);
    m_listLayout->addWidget(m_playlistListEditor);
}

void KNMusicPlaylistView::onActionAddPlaylist()
{
    onActionEditPlaylist(m_manager->createPlaylist(tr("New Playlist")));
}

void KNMusicPlaylistView::onActionRemoveCurrentPlaylist()
{
    m_manager->removePlaylist(m_currentPath);
}

void KNMusicPlaylistView::onActionEditPlaylist(const QModelIndex &index)
{
    m_playlistListView->setCurrentIndex(index);
    m_playlistListView->edit(index);
}

void KNMusicPlaylistView::onActionShowPlaylist(const QModelIndex &index)
{
    m_displayer->setPlaylistName(m_manager->playlistName(index));
    m_songsView->setModel(m_manager->playlistDataModel(index));
    m_currentPath=m_manager->playlistPath(index.row());
}

void KNMusicPlaylistView::onActionOpenUrl(const QModelIndex &index)
{
    m_manager->setPlaylist(m_currentPath);
    m_manager->setCurrentPlaylistPlaying(index);
    emit requirePlayMusic(m_manager->filePathFromIndex(index));
}

void KNMusicPlaylistView::onActionItemUpdate(const QModelIndex &index)
{
    QString updateFilePath=m_manager->playlistPath(index.row());
    if(m_currentPath==updateFilePath)
    {
        onActionShowPlaylist(index);
    }
}
