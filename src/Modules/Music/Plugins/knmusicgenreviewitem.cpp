#include "../Libraries/knmusicgenremodel.h"
#include "../Libraries/knmusiccategorydetailmodel.h"
#include "../Libraries/knmusiccategorysortfiltermodel.h"

#include "../Widgets/knmusiccategoryview.h"
#include "../Widgets/knmusicgenresongs.h"

#include "knmusicgenreviewitem.h"

KNMusicGenreViewItem::KNMusicGenreViewItem(QObject *parent) :
    KNMusicViewerItemBase(parent)
{
    //Initial translation and icons.
    retranslate();

    //Initial the genre model.
    m_genreModel=new KNMusicGenreModel;

    //Initial the sort filter proxy model for genre search.
    m_genreSortModel=new KNMusicCategorySortFilterModel;
    m_genreSortModel->setFilterKeyColumn(0);
    m_genreSortModel->setFilterCaseSensitivity(Qt::CaseInsensitive);
    m_genreSortModel->setSourceModel(m_genreModel);

    //Initial the songs display filter model.
    m_genreDetails=new KNMusicCategoryDetailModel;
    m_genreDetails->setFilterKeyColumn(KNMusicGlobal::Genre);
    m_genreDetails->setCategoryModel(m_genreModel);

    //Initial the genre view.
    m_genreView=new KNMusicCategoryView;
    //Initial the genre songs view.
    m_genreSongView=new KNMusicGenreSongs(m_genreView);
    connect(m_genreSongView, &KNMusicGenreSongs::requireOpenUrl,
            [=](const QModelIndex &index)
            {
                emit requireSetProxy(m_genreDetails);
                emit requirePlayMusic(m_genreDetails->mapToSource(index));
            });
    connect(m_genreSongView, &KNMusicGenreSongs::requireShowContextMenu,
            this, &KNMusicGenreViewItem::onActionShowContextMenu);

    m_genreView->setSongListView(m_genreSongView);
    m_genreView->setModel(m_genreSortModel);
    m_genreView->setDetailModel(m_genreDetails);
    connect(m_genreView, &KNMusicCategoryView::requireAnalysisUrls,
            this, &KNMusicGenreViewItem::requireAnalysisUrls);
    connect(m_genreView, &KNMusicCategoryView::dragEntered,
            this, &KNMusicGenreViewItem::dragEntered);
    connect(m_genreView, &KNMusicCategoryView::dropped,
            this, &KNMusicGenreViewItem::dropped);
}

KNMusicGenreViewItem::~KNMusicGenreViewItem()
{
    m_genreModel->deleteLater();
    m_genreSortModel->deleteLater();
    m_genreDetails->deleteLater();
    m_genreView->deleteLater();
}

void KNMusicGenreViewItem::applyPlugin()
{
    emit requireAddCategory(QPixmap(":/Category/Resources/Category/04_genres.png"),
                            m_captionTitle,
                            m_genreView);
}

void KNMusicGenreViewItem::onActionResort()
{
    m_genreSortModel->sort(0);
}

void KNMusicGenreViewItem::onActionSearch(const QString &text)
{
    //Use filter instead of search
    m_genreSortModel->setFilterFixedString(text);
}

void KNMusicGenreViewItem::onActionShowIndex(const QModelIndex &index)
{
    m_genreView->selectMusicItem(index);
}

void KNMusicGenreViewItem::onActionRemoveItem(const QModelIndex &index)
{
    m_genreModel->onMusicRemoved(index);
}

void KNMusicGenreViewItem::retranslate()
{
    m_captionTitle=tr("Genre");
}

void KNMusicGenreViewItem::setMusicSourceModel(KNMusicLibraryModelBase *model)
{
    m_genreModel->setSourceModel(model);
    m_genreView->setMusicSourceModel(model);
    m_genreView->resetHeader();
}

void KNMusicGenreViewItem::setBackend(KNMusicBackend *backend)
{
    m_genreSongView->setMusicBackend(backend);
}

void KNMusicGenreViewItem::onActionShowContextMenu(const QPoint &position)
{
    emit requireShowContextMenu(position, KNMusicGlobal::GenreView);
}
