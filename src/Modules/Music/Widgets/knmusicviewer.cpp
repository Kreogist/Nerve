#include <QDragEnterEvent>
#include <QDropEvent>
#include <QMimeData>
#include <QModelIndex>

#include <QStyleFactory>

#include <QDebug>

#include "knmusiclistview.h"
#include "knmusicartistview.h"

#include "../Libraries/knmusicsortmodel.h"
#include "../Libraries/knmusicartistmodel.h"
#include "../Libraries/knmusicartistdetailmodel.h"
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

    m_detailModel=new KNMusicArtistDetailModel(this);
    m_detailModel->setArtistModel(m_artistModel);
    m_artistView->setDetailModel(m_detailModel);
    connect(m_detailModel, &KNMusicArtistDetailModel::requireDetailSizeChange,
            m_artistView, &KNMusicArtistView::onActionDetailCountChange);

    QWidget *empty2=new QWidget(this),
            *empty3=new QWidget(this),
            *empty4=new QWidget(this);

    addCategory(QPixmap(":/Category/Resources/Category/01_musics.png"),
                m_categoryCaption[Songs],
                m_libraryView);
    addCategory(QPixmap(":/Category/Resources/Category/02_artists.png"),
                m_categoryCaption[Artists],
                m_artistView);
    addCategory(QPixmap(":/Category/Resources/Category/03_ablums.png"),
                m_categoryCaption[Albums],
                empty2);
    addCategory(QPixmap(":/Category/Resources/Category/04_genres.png"),
                m_categoryCaption[Genres],
                empty3);
    addCategory(QPixmap(":/Category/Resources/Category/05_playlists.png"),
                m_categoryCaption[Playlists],
                empty4);
}

void KNMusicViewer::setModel(QAbstractItemModel *model)
{
    m_listViewModel->setSourceModel(model);
    m_artistModel->setSourceModel(model);
    m_detailModel->setSourceModel(model);
    m_libraryView->resetHeader();
    m_artistView->resetHeader();
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
    emit requireOpenUrl(m_detailModel->mapToSource(index));
}
