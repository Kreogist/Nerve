#include "../Libraries/knmusicsortmodel.h"
#include "../Widgets/knmusiclistview.h"

#include "knmusiclistviewitem.h"

KNMusicListViewItem::KNMusicListViewItem(QObject *parent) :
    KNMusicViewerItemBase(parent)
{
    //Initial translation and icons.
    retranslate();

    //Initial the sort model.
    m_listViewModel=new KNMusicSortModel;
    m_listViewModel->setFilterKeyColumn(-1);
    m_listViewModel->setFilterCaseSensitivity(Qt::CaseInsensitive);
    m_listViewModel->setSortCaseSensitivity(Qt::CaseInsensitive);

    //Initial the view widget.
    m_libraryView=new KNMusicListView;
    m_libraryView->setModel(m_listViewModel);
    m_libraryView->installEventFilter(this);
    connect(m_libraryView, &KNMusicListView::requireOpenUrl,
            [=](const QModelIndex &index)
            {
                emit requireSetProxy(m_listViewModel);
                emit requirePlayMusic(m_listViewModel->mapToSource(index));
            });
    connect(m_libraryView, &KNMusicListView::requireShowContextMenu,
            this, &KNMusicListViewItem::onActionShowContextMenu);
}

KNMusicListViewItem::~KNMusicListViewItem()
{
    m_libraryView->deleteLater();
    m_listViewModel->deleteLater();
}

void KNMusicListViewItem::setMusicSourceModel(KNMusicLibraryModelBase *model)
{
    //Set the music source model.
    m_libraryView->setSourceModel(model);
    m_listViewModel->setSourceModel(model);
    //Initial the header.
    m_libraryView->resetHeader();
}

void KNMusicListViewItem::setBackend(KNMusicBackend *backend)
{
    m_libraryView->setMusicBackend(backend);
}

void KNMusicListViewItem::applyPlugin()
{
    emit requireAddCategory(QPixmap(":/Category/Resources/Category/01_musics.png"),
                            m_captionTitle,
                            m_libraryView);
}

void KNMusicListViewItem::onActionResort()
{
    m_listViewModel->sort(0);
}

void KNMusicListViewItem::onActionSearch(const QString &text)
{
    m_listViewModel->setFilterFixedString(text);
}

void KNMusicListViewItem::onActionShowIndex(const QModelIndex &index)
{
    //Get the proxy index.
    QModelIndex songMappedIndex=m_listViewModel->mapFromSource(index);
    //Select the index and show it.
    m_libraryView->setCurrentIndex(songMappedIndex);
    m_libraryView->scrollTo(songMappedIndex, QAbstractItemView::PositionAtCenter);
}

void KNMusicListViewItem::onActionRemoveOriginalItem(const QModelIndex &index)
{
    m_listViewModel->removeOriginalItem(index);
}

void KNMusicListViewItem::retranslate()
{
    m_captionTitle=tr("Songs");
}

void KNMusicListViewItem::onActionShowContextMenu(const QPoint &position)
{
    emit requireShowContextMenu(position, KNMusicGlobal::SongsView);
}
