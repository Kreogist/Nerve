#include <QDragEnterEvent>
#include <QDropEvent>
#include <QMimeData>
#include <QModelIndex>

#include <QStyleFactory>

#include <QDebug>

#include "knmusiclistview.h"
#include "knmusicartistview.h"
#include "knmusicalbumview.h"

#include "../Libraries/knmusicsortmodel.h"
#include "../Libraries/knmusicartistmodel.h"
#include "../Libraries/knmusicalbummodel.h"
#include "../Libraries/knmusicgenremodel.h"
#include "../Libraries/knmusiccategorydetailmodel.h"
#include "../knmusicglobal.h"

#include "knmusicviewer.h"

KNMusicViewer::KNMusicViewer(QWidget *parent) :
    KNStdLibViewer(parent)
{
    retranslate();
    setContentsMargins(0,0,0,0);
    setAcceptDrops(true);

    m_listViewModel=new KNMusicSortModel(this);
    m_artistModel=new KNMusicArtistModel(this);
    m_albumModel=new KNMusicAlbumModel(this);
    m_genreModel=new KNMusicGenreModel(this);

    m_libraryView=new KNMusicListView(this);
    m_libraryView->setModel(m_listViewModel);
    connect(m_libraryView, SIGNAL(requireOpenUrl(QModelIndex)),
            this, SLOT(onActionListviewOpenUrl(QModelIndex)));
    connect(m_libraryView, &KNMusicListView::requireShowContextMenu,
            this, &KNMusicViewer::onActionLibraryViewShowContextMenu);

    m_artistView=new KNMusicArtistView(this);
    connect(m_artistView, &KNMusicArtistView::requireOpenUrl,
            this, &KNMusicViewer::onActionArtistOpenUrl);
    m_artistView->setModel(m_artistModel);

    m_albumView=new KNMusicAlbumView(this);
    m_albumView->setModel(m_albumModel);

    m_genreView=new KNMusicArtistView(this);
    connect(m_genreView, &KNMusicArtistView::requireOpenUrl,
            this, &KNMusicViewer::onActionGenreOpenUrl);
    m_genreView->setModel(m_genreModel);

    m_artistDetails=new KNMusicCategoryDetailModel(this);
    m_artistDetails->setFilterKeyColumn(KNMusicGlobal::Artist);
    m_artistDetails->setCategoryModel(m_artistModel);
    m_artistView->setDetailModel(m_artistDetails);
    connect(m_artistDetails, &KNMusicCategoryDetailModel::requireDetailSizeChange,
            m_artistView, &KNMusicArtistView::onActionDetailSizeChange);

    m_genreDetails=new KNMusicCategoryDetailModel(this);
    m_genreDetails->setFilterKeyColumn(KNMusicGlobal::Genre);
    m_genreDetails->setCategoryModel(m_genreModel);
    m_genreView->setDetailModel(m_genreDetails);
    connect(m_genreDetails, &KNMusicCategoryDetailModel::requireDetailSizeChange,
            m_genreView, &KNMusicArtistView::onActionDetailSizeChange);

    QWidget *empty4=new QWidget(this);

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
    addCategory(QPixmap(":/Category/Resources/Category/05_playlists.png"),
                m_categoryCaption[Playlists],
                empty4);
}

void KNMusicViewer::setModel(QAbstractItemModel *model)
{
    m_listViewModel->setSourceModel(model);
    m_artistModel->setSourceModel(model);
    m_artistDetails->setSourceModel(model);
    m_albumModel->setSourceModel(model);
    m_genreModel->setSourceModel(model);
    m_genreDetails->setSourceModel(model);
    m_libraryView->resetHeader();
    m_artistView->resetHeader();
    m_genreView->resetHeader();
}

void KNMusicViewer::retranslate()
{
    m_categoryCaption[Songs]=tr("Songs");
    m_categoryCaption[Artists]=tr("Artists");
    m_categoryCaption[Albums]=tr("Albums");
    m_categoryCaption[Genres]=tr("Genres");
    m_categoryCaption[Playlists]=tr("Playlists");
}

void KNMusicViewer::retranslateAndSet()
{
    retranslate();
}

void KNMusicViewer::resort()
{
    m_artistView->resort();
    m_genreView->resort();
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
    emit requireAnalysisUrls(event->mimeData()->urls());
}

void KNMusicViewer::onActionLibraryViewShowContextMenu(const QPoint &position,
                                                       const QModelIndex &index)
{
    QModelIndex sourceIndex=m_listViewModel->mapToSource(index);
    emit requireShowContextMenu(position, sourceIndex);
}

void KNMusicViewer::onActionListviewOpenUrl(const QModelIndex &index)
{
    emit requireOpenUrl(m_listViewModel->mapToSource(index));
}

void KNMusicViewer::onActionArtistOpenUrl(const QModelIndex &index)
{
    emit requireOpenUrl(m_artistDetails->mapToSource(index));
}

void KNMusicViewer::onActionGenreOpenUrl(const QModelIndex &index)
{
    emit requireOpenUrl(m_genreDetails->mapToSource(index));
}
