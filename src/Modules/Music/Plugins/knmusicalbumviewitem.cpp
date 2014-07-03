#include "../Libraries/knmusicalbummodel.h"
#include "../Libraries/knmusicalbumdetailmodel.h"
#include "../Libraries/knmusiccategorysortfiltermodel.h"

#include "../Widgets/knmusicalbumview.h"
#include "../Widgets/knmusicviewcontainer.h"

#include "knmusicalbumviewitem.h"

KNMusicAlbumViewItem::KNMusicAlbumViewItem(QObject *parent) :
    KNMusicViewerItemBase(parent)
{
    //Initial translation and icons.
    retranslate();

    //Initial the album model.
    m_albumModel=new KNMusicAlbumModel;

    //Initial the sort filter proxy model for album search.
    m_albumSortModel=new KNMusicCategorySortFilterModel;
    m_albumSortModel->setFilterKeyColumn(0);
    m_albumSortModel->setFilterCaseSensitivity(Qt::CaseInsensitive);
    m_albumSortModel->setSourceModel(m_albumModel);

    //Initial the songs display filter model.
    m_albumDetails=new KNMusicAlbumDetailModel;
    m_albumDetails->setFilterKeyColumn(KNMusicGlobal::Album);
    m_albumDetails->setCategoryModel(m_albumModel);

    //Initial the album view.
    m_albumView=new KNMusicAlbumView;

    m_albumView->setCategoryModel(m_albumSortModel);
    m_albumView->setDetailModel(m_albumDetails);
    connect(m_albumView, &KNMusicAlbumView::requireOpenUrl,
            [=](const QModelIndex &index)
            {
                emit requireSetProxy(m_albumDetails);
                emit requirePlayMusic(m_albumDetails->mapToSource(index));
            });
    connect(m_albumView, &KNMusicAlbumView::requireShowContextMenu,
            this, &KNMusicAlbumViewItem::onActionShowContextMenu);

    //Initial the container.
    m_container=new KNMusicViewContainer;
    m_container->setCentralWidget(m_albumView);
    connect(m_container, &KNMusicViewContainer::requireAnalysisUrls,
            this, &KNMusicAlbumViewItem::requireAnalysisUrls);
    connect(m_container, &KNMusicViewContainer::dragEntered,
            this, &KNMusicAlbumViewItem::dragEntered);
    connect(m_container, &KNMusicViewContainer::dropped,
            this, &KNMusicAlbumViewItem::dropped);
}

KNMusicAlbumViewItem::~KNMusicAlbumViewItem()
{
    m_albumModel->deleteLater();
    m_albumSortModel->deleteLater();
    m_albumDetails->deleteLater();
    m_albumView->deleteLater();
}

void KNMusicAlbumViewItem::applyPlugin()
{
    emit requireAddCategory(QPixmap(":/Category/Resources/Category/03_ablums.png"),
                            m_captionTitle,
                            m_container);
}

void KNMusicAlbumViewItem::onActionResort()
{
    m_albumSortModel->sort(0);
}

void KNMusicAlbumViewItem::onActionSearch(const QString &text)
{
    m_albumView->setFilterFixedString(text);
}

void KNMusicAlbumViewItem::onActionShowIndex(const QModelIndex &index)
{
    m_albumView->selectMusicItem(index);
}

void KNMusicAlbumViewItem::onActionRemoveItem(const QModelIndex &index)
{
    m_albumModel->onMusicRemoved(index);
}

void KNMusicAlbumViewItem::retranslate()
{
    m_captionTitle=tr("Albums");
}

void KNMusicAlbumViewItem::setMusicSourceModel(KNMusicLibraryModelBase *model)
{
    m_albumModel->setSourceModel(model);
    m_albumView->setMusicSourceModel(model);
    m_albumView->resetHeader();
}

void KNMusicAlbumViewItem::setBackend(KNMusicBackend *backend)
{
    m_albumView->setMusicBackend(backend);
}

void KNMusicAlbumViewItem::onActionShowContextMenu(const QPoint &position)
{
    emit requireShowContextMenu(position,
                                KNMusicGlobal::AlbumView);
}
