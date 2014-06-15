#include <QTreeView>

#include "../Libraries/knmusicplaylistmanager.h"

#include "knmusicplaylistview.h"

KNMusicPlaylistView::KNMusicPlaylistView(QWidget *parent) :
    QSplitter(parent)
{
    m_playlistListView=new QTreeView(this);
    addWidget(m_playlistListView);
}

void KNMusicPlaylistView::setManager(KNMusicPlaylistManager *manager)
{
    m_manager=manager;
    connect(m_manager, &KNMusicPlaylistManager::requireUpdatePlaylistModel,
            this, &KNMusicPlaylistView::setPlaylistList);
}

void KNMusicPlaylistView::setPlaylistList(QAbstractItemModel *model)
{
    m_playlistListView->setModel(model);
}
