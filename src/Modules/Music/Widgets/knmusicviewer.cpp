#include <QAbstractItemModel>
#include <QDragEnterEvent>
#include <QDropEvent>
#include <QEvent>
#include <QKeyEvent>
#include <QMimeData>
#include <QModelIndex>

#include <QStyleFactory>

#include <QDebug>

#include "knmusiclistview.h"
#include "knmusicartistview.h"
#include "knmusicalbumview.h"
#include "knmusicartistsongs.h"
#include "knmusicgenresongs.h"

#include "../../Base/knlibfilter.h"

#include "../Libraries/knmusicmodel.h"
#include "../Libraries/knmusicsortmodel.h"
#include "../Libraries/knmusicartistmodel.h"
#include "../Libraries/knmusicalbummodel.h"
#include "../Libraries/knmusicgenremodel.h"
#include "../Libraries/knmusiccategorydetailmodel.h"
#include "../Libraries/knmusicalbumdetailmodel.h"
#include "../Libraries/knmusiccategorysortfiltermodel.h"

#include "knmusicviewer.h"

KNMusicViewer::KNMusicViewer(QWidget *parent) :
    KNStdLibViewer(parent)
{
    retranslate();
    setContentsMargins(0,0,0,0);
    setAcceptDrops(true);

    m_listViewModel=new KNMusicSortModel(this);
    m_listViewModel->setFilterKeyColumn(-1);
    m_listViewModel->setFilterCaseSensitivity(Qt::CaseInsensitive);
    m_listViewModel->setSortCaseSensitivity(Qt::CaseInsensitive);
    m_artistModel=new KNMusicArtistModel(this);
    m_albumModel=new KNMusicAlbumModel(this);
    m_genreModel=new KNMusicGenreModel(this);

    m_albumSortModel=new KNMusicCategorySortFilterModel(this);
    m_albumSortModel->setFilterKeyColumn(-1);
    m_albumSortModel->setFilterCaseSensitivity(Qt::CaseInsensitive);
    m_albumSortModel->setSourceModel(m_albumModel);

    m_libraryView=new KNMusicListView(this);
    m_libraryView->setModel(m_listViewModel);
    m_libraryView->installEventFilter(this);
    connect(m_libraryView, &KNMusicListView::requireOpenUrl,
            this, &KNMusicViewer::onActionListviewOpenUrl);
    connect(m_libraryView, &KNMusicListView::requireShowContextMenu,
            this, &KNMusicViewer::onActionLibraryViewShowContextMenu);

    m_artistView=new KNMusicArtistView(this);
    m_artistSongView=new KNMusicArtistSongs(m_artistView);
    m_artistView->installEventFilter(this);
    m_artistSongView->installEventFilter(this);
    m_artistView->setSongListView(m_artistSongView);
    m_artistView->setModel(m_artistModel);
    connect(m_artistSongView, &KNMusicArtistSongs::requireOpenUrl,
            this, &KNMusicViewer::onActionArtistOpenUrl);
    connect(m_artistSongView, &KNMusicArtistSongs::requireShowContextMenu,
            this, &KNMusicViewer::onActionArtistShowContextMenu);

    m_albumView=new KNMusicAlbumView(this);
    m_albumView->setCategoryModel(m_albumSortModel);
    m_albumView->installEventFilter(this);
    connect(m_albumView, &KNMusicAlbumView::requireOpenUrl,
            this, &KNMusicViewer::onActionAlbumOpenUrl);
    connect(m_albumView, &KNMusicAlbumView::requireShowContextMenu,
            this, &KNMusicViewer::onActionAlbumShowContextMenu);

    m_genreView=new KNMusicArtistView(this);
    m_genreSongView=new KNMusicGenreSongs(m_genreView);
    m_genreView->installEventFilter(this);
    m_genreSongView->installEventFilter(this);
    m_genreView->setSongListView(m_genreSongView);
    m_genreView->setModel(m_genreModel);
    connect(m_genreSongView, &KNMusicGenreSongs::requireOpenUrl,
            this, &KNMusicViewer::onActionGenreOpenUrl);
    connect(m_genreSongView, &KNMusicGenreSongs::requireShowContextMenu,
            this, &KNMusicViewer::onActionGenreShowContextMenu);

    m_artistDetails=new KNMusicCategoryDetailModel(this);
    m_artistDetails->setFilterKeyColumn(KNMusicGlobal::Artist);
    m_artistDetails->setCategoryModel(m_artistModel);
    m_artistView->setDetailModel(m_artistDetails);

    m_albumDetails=new KNMusicAlbumDetailModel(this);
    m_albumDetails->setFilterKeyColumn(KNMusicGlobal::Album);
    m_albumDetails->setCategoryModel(m_albumModel);
    m_albumView->setDetailModel(m_albumDetails);

    m_genreDetails=new KNMusicCategoryDetailModel(this);
    m_genreDetails->setFilterKeyColumn(KNMusicGlobal::Genre);
    m_genreDetails->setCategoryModel(m_genreModel);
    m_genreView->setDetailModel(m_genreDetails);

    addCategory(QPixmap(":/Category/Resources/Category/01_musics.png"),
                m_categoryCaption[Songs],
                m_libraryView);
    addCategory(QPixmap(":/Category/Resources/Category/02_artists.png"),
                m_categoryCaption[Artists],
                m_artistView);
    addCategory(QPixmap(":/Category/Resources/Category/03_ablums.png"),
                m_categoryCaption[Albums],
                m_albumView);
    addCategory(QPixmap(":/Category/Resources/Category/04_genres.png"),
                m_categoryCaption[Genres],
                m_genreView);
}

void KNMusicViewer::setModel(KNMusicModel *model)
{
    m_listViewModel->setSourceModel(model);
    m_artistModel->setSourceModel(model);
    m_artistDetails->setSourceModel(model);
    m_albumModel->setSourceModel(model);
    m_albumDetails->setSourceModel(model);
    m_genreModel->setSourceModel(model);
    m_genreDetails->setSourceModel(model);
    m_libraryView->resetHeader();
    m_artistView->resetHeader();
    m_albumView->resetHeader();
    m_genreView->resetHeader();
    m_sourceModel=model;
}

bool KNMusicViewer::eventFilter(QObject *watched, QEvent *event)
{
    /*if(event->type()==QEvent::KeyRelease)
    {
        QKeyEvent *releasedKey=static_cast<QKeyEvent *>(event);
        if(releasedKey->modifiers()==Qt::ControlModifier)
        {
            switch(releasedKey->key())
            {
            case Qt::Key_Left:
                moveLeft();
                return true;
            case Qt::Key_Right:
                moveRight();
                return true;
            default:
                return KNStdLibViewer::eventFilter(watched, event);
            }
        }
    }*/
    return KNStdLibViewer::eventFilter(watched, event);
}

void KNMusicViewer::retranslate()
{
    m_categoryCaption[Songs]=tr("Songs");
    m_categoryCaption[Artists]=tr("Artists");
    m_categoryCaption[Albums]=tr("Albums");
    m_categoryCaption[Genres]=tr("Genres");
}

void KNMusicViewer::retranslateAndSet()
{
    retranslate();
}

void KNMusicViewer::resort()
{
    m_listViewModel->sort(0);
    m_artistModel->sort(0);
    m_albumModel->sort(0);
    m_genreModel->sort(0);
}

void KNMusicViewer::showIn(KNMusicGlobal::MusicCategory category,
                           const QModelIndex &index)
{
    switch(category)
    {
    case KNMusicGlobal::SongsView:
        m_libraryView->setCurrentIndex(m_listViewModel->mapFromSource(index));
        break;
    case KNMusicGlobal::ArtistView:
        m_artistView->selectCategoryItem(m_sourceModel->data(m_sourceModel->index(index.row(),
                                                                                  KNMusicGlobal::Artist)).toString());
        m_artistView->selectItem(index);
        break;
    case KNMusicGlobal::AlbumView:
        m_albumView->selectCategoryItem(m_sourceModel->data(m_sourceModel->index(index.row(),
                                                                                 KNMusicGlobal::Album)).toString());
        m_albumView->selectItem(index);
        break;
    case KNMusicGlobal::GenreView:
        m_genreView->selectCategoryItem(m_sourceModel->data(m_sourceModel->index(index.row(),
                                                                                 KNMusicGlobal::Genre)).toString());
        m_genreView->selectItem(index);
        break;
    }
    setCategoryIndex(category);
}

void KNMusicViewer::deleteMusic(const QModelIndex &index)
{
    m_artistModel->onMusicRemoved(index);
    m_albumModel->onMusicRemoved(index);
    m_genreModel->onMusicRemoved(index);
    m_sourceModel->prepareRemove(index);
    m_listViewModel->removeOriginalItem(index);
}

void KNMusicViewer::onActionSearch(const QString &text)
{
    m_listViewModel->setFilterFixedString(text);
    m_albumView->setFilterFixedString(text);
}

void KNMusicViewer::dragEnterEvent(QDragEnterEvent *event)
{
    if(event->mimeData()->hasUrls())
    {
        event->acceptProposedAction();
    }
}

void KNMusicViewer::dropEvent(QDropEvent *event)
{
    m_filter->analysisList(event->mimeData()->urls());
}

void KNMusicViewer::onActionLibraryViewShowContextMenu(const QPoint &position,
                                                       const QModelIndex &index)
{
    emit requireShowContextMenu(position,
                                m_listViewModel->mapToSource(index),
                                KNMusicGlobal::SongsView);
}

void KNMusicViewer::onActionArtistShowContextMenu(const QPoint &position,
                                                  const QModelIndex &index)
{
    emit requireShowContextMenu(position,
                                m_artistDetails->mapToSource(index),
                                KNMusicGlobal::ArtistView);
}

void KNMusicViewer::onActionAlbumShowContextMenu(const QPoint &position,
                                                 const QModelIndex &index)
{
    emit requireShowContextMenu(position,
                                m_albumDetails->mapToSource(index),
                                KNMusicGlobal::AlbumView);
}

void KNMusicViewer::onActionGenreShowContextMenu(const QPoint &position,
                                                 const QModelIndex &index)
{
    emit requireShowContextMenu(position,
                                m_genreDetails->mapToSource(index),
                                KNMusicGlobal::GenreView);
}

void KNMusicViewer::onActionListviewOpenUrl(const QModelIndex &index)
{
    emit requireOpenUrl(m_listViewModel->mapToSource(index));
}

void KNMusicViewer::onActionArtistOpenUrl(const QModelIndex &index)
{
    emit requireOpenUrl(m_artistDetails->mapToSource(index));
}

void KNMusicViewer::onActionAlbumOpenUrl(const QModelIndex &index)
{
    emit requireOpenUrl(m_albumDetails->mapToSource(index));
}

void KNMusicViewer::onActionGenreOpenUrl(const QModelIndex &index)
{
    emit requireOpenUrl(m_genreDetails->mapToSource(index));
}
