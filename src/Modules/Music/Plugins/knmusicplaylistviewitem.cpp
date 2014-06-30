#include "../Widgets/knmusicplaylistview.h"

#include "knmusicplaylistviewitem.h"

KNMusicPlaylistViewItem::KNMusicPlaylistViewItem(QObject *parent) :
    KNMusicViewerPlaylistItemBase(parent)
{
    retranslate();
    m_playlistView=new KNMusicPlaylistView;
    connect(m_playlistView, &KNMusicPlaylistView::requirePlayMusic,
            this, &KNMusicPlaylistViewItem::requirePlayMusic);
}

KNMusicPlaylistViewItem::~KNMusicPlaylistViewItem()
{
    m_playlistView->deleteLater();
}

void KNMusicPlaylistViewItem::applyPlugin()
{
    emit requireAddCategory(QPixmap(":/Category/Resources/Category/05_playlists.png"),
                            m_title,
                            m_playlistView);
}

void KNMusicPlaylistViewItem::setPlaylistManager(KNMusicPlaylistManagerBase *manager)
{
    m_playlistView->setManager(manager);
}

void KNMusicPlaylistViewItem::retranslate()
{
    m_title=tr("Playlists");
}
