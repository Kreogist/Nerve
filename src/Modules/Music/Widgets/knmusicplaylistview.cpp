#include <QDebug>

#include "knmusicplaylistlistview.h"
#include "knmusicplaylistdisplay.h"
#include "knmusicplaylistsongs.h"
#include "../Libraries/knmusicplaylistmanager.h"

#include "knmusicplaylistview.h"

KNMusicPlaylistView::KNMusicPlaylistView(QWidget *parent) :
    QSplitter(parent)
{
    m_playlistListView=new KNMusicPlaylistListview(this);
    addWidget(m_playlistListView);
    m_displayer=new KNMusicPlaylistDisplay(this);
    addWidget(m_displayer);
    m_songsView=new KNMusicPlaylistSongs(this);
    m_displayer->setSongsView(m_songsView);
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
