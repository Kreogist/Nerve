#include "../Libraries/knmusicartistmodel.h"
#include "../Libraries/knmusiccategorydetailmodel.h"
#include "../Libraries/knmusiccategorysortfiltermodel.h"

#include "../Widgets/knmusiccategoryview.h"
#include "../Widgets/knmusicartistsongs.h"

#include "knmusicartistviewitem.h"

KNMusicArtistViewItem::KNMusicArtistViewItem(QObject *parent) :
    KNMusicViewerItem(parent)
{
    //Initial translation and icons.
    retranslate();

    //Initial the artist model.
    m_artistModel=new KNMusicArtistModel;

    //Initial the sort filter proxy model for artist search.
    m_artistSortModel=new KNMusicCategorySortFilterModel;
    m_artistSortModel->setFilterKeyColumn(0);
    m_artistSortModel->setFilterCaseSensitivity(Qt::CaseInsensitive);
    m_artistSortModel->setSourceModel(m_artistModel);

    //Initial the songs display filter model.
    m_artistDetails=new KNMusicCategoryDetailModel;
    m_artistDetails->setFilterKeyColumn(KNMusicGlobal::Artist);
    m_artistDetails->setCategoryModel(m_artistModel);

    //Initial the artist view.
    m_artistView=new KNMusicCategoryView;
    //Initial the artist songs view.
    m_artistSongView=new KNMusicArtistSongs(m_artistView);

    m_artistView->setSongListView(m_artistSongView);
    m_artistView->setModel(m_artistSortModel);
    m_artistView->setDetailModel(m_artistDetails);
    connect(m_artistSongView, &KNMusicArtistSongs::requireOpenUrl,
            [=](const QModelIndex &index)
            {
                emit requireSetProxy(m_artistDetails);
                emit requirePlayMusic(m_artistDetails->mapToSource(index));
            });
    connect(m_artistSongView, &KNMusicArtistSongs::requireShowContextMenu,
            this, &KNMusicArtistViewItem::onActionShowContextMenu);
}

KNMusicArtistViewItem::~KNMusicArtistViewItem()
{
    m_artistModel->deleteLater();
    m_artistSortModel->deleteLater();
    m_artistDetails->deleteLater();
    m_artistView->deleteLater();
}

void KNMusicArtistViewItem::applyPlugin()
{
    emit requireAddCategory(QPixmap(":/Category/Resources/Category/02_artists.png"),
                            m_captionTitle,
                            m_artistView);
}

void KNMusicArtistViewItem::onActionResort()
{
    m_artistSortModel->sort(0);
}

void KNMusicArtistViewItem::onActionSearch(const QString &text)
{
    //Use filter instead of search
    m_artistSortModel->setFilterFixedString(text);
}

void KNMusicArtistViewItem::onActionShowIndex(const QModelIndex &index)
{
    m_artistView->selectMusicItem(index);
}

void KNMusicArtistViewItem::onActionRemoveItem(const QModelIndex &index)
{
    m_artistModel->onMusicRemoved(index);
}

void KNMusicArtistViewItem::retranslate()
{
    m_captionTitle=tr("Artists");
}

void KNMusicArtistViewItem::setMusicSourceModel(KNMusicLibraryModelBase *model)
{
    m_artistModel->setSourceModel(model);
    m_artistView->setMusicSourceModel(model);
    m_artistView->resetHeader();
}

void KNMusicArtistViewItem::setBackend(KNMusicBackend *backend)
{
    m_artistSongView->setMusicBackend(backend);
}

void KNMusicArtistViewItem::onActionShowContextMenu(const QPoint &position)
{
    emit requireShowContextMenu(position, KNMusicGlobal::ArtistView);
}
