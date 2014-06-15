#include <QDebug>

#include "knmusicplaylistlistview.h"
#include "../Libraries/knmusicplaylistmanager.h"

#include "knmusicplaylistview.h"

KNMusicPlaylistView::KNMusicPlaylistView(QWidget *parent) :
    QSplitter(parent)
{
    m_playlistListView=new KNMusicPlaylistListview(this);
    addWidget(m_playlistListView);
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
    qDebug()<<index.row();
}
