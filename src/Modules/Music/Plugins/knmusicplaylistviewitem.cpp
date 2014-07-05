#include <QDebug>

#include "../Base/knmusicplaylistmanagerbase.h"

#include "../Widgets/knmusicplaylistdraglistview.h"
#include "../Widgets/knmusicplaylistview.h"

#include "knmusicplaylistviewitem.h"

KNMusicPlaylistViewItem::KNMusicPlaylistViewItem(QObject *parent) :
    KNMusicViewerPlaylistItemBase(parent)
{
    //Get the translation.
    retranslate();

    //Prepare the view widget.
    m_playlistView=new KNMusicPlaylistView;
    connect(m_playlistView, &KNMusicPlaylistView::requirePlayMusic,
            this, &KNMusicPlaylistViewItem::requirePlayMusic);

    //Prepare the playlist list view.
    m_dragListView=new KNMusicPlaylistDragListView;
    m_dragListView->setDragEnabled(true);
    m_dragListView->setAcceptDrops(true);
    m_dragListView->setDropIndicatorShown(true);
}

KNMusicPlaylistViewItem::~KNMusicPlaylistViewItem()
{
    m_playlistView->deleteLater();
    if(m_dragListView->parent()==0)
    {
        m_dragListView->deleteLater();
    }
}

void KNMusicPlaylistViewItem::applyPlugin()
{
    emit requireAddCategory(QPixmap(":/Category/Resources/Category/05_playlists.png"),
                            m_title,
                            m_playlistView);
}

QWidget *KNMusicPlaylistViewItem::playlistListView()
{
    return m_dragListView;
}

void KNMusicPlaylistViewItem::setPlaylistManager(KNMusicPlaylistManagerBase *manager)
{
    m_playlistView->setManager(manager);
    m_dragListView->setModel(manager->playlistModel());
    connect(manager, &KNMusicPlaylistManagerBase::requireRenameRow,
            this, &KNMusicPlaylistViewItem::onActionRenameList);
}

void KNMusicPlaylistViewItem::onActionRenameList(const QModelIndex &index)
{
    emit requireSwitchToPlaylist();
    m_playlistView->onActionEditPlaylist(index);
}

void KNMusicPlaylistViewItem::retranslate()
{
    m_title=tr("Playlists");
}
