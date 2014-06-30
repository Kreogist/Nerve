#include <QSignalMapper>
#include <QAbstractItemModel>
#include <QDragEnterEvent>
#include <QDropEvent>
#include <QEvent>
#include <QSignalMapper>
#include <QMouseEvent>
#include <QKeyEvent>
#include <QMimeData>
#include <QModelIndex>
#include <QResizeEvent>
#include <QPropertyAnimation>

#include <QStyleFactory>

#include <QDebug>

#include "../Base/knmusicvieweritem.h"

#include "knmusiccategoryview.h"
#include "knmusicalbumview.h"
#include "knmusicplaylistview.h"
#include "knmusicgenresongs.h"

#include "../../Base/knlibsearcher.h"

#include "../Plugins/knmusiclistviewitem.h"

#include "../Libraries/knmusiclibrarymodel.h"
#include "../Libraries/knmusicsortmodel.h"
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

    //Prepare show in signal mapper.
    m_showInMapper=new QSignalMapper(this);

    addListviewModePlugin(new KNMusicListViewItem);

    m_albumModel=new KNMusicAlbumModel;
    m_genreModel=new KNMusicGenreModel;

    m_albumSortModel=new KNMusicCategorySortFilterModel;
    m_albumSortModel->setFilterKeyColumn(0);
    m_albumSortModel->setFilterCaseSensitivity(Qt::CaseInsensitive);
    m_albumSortModel->setSourceModel(m_albumModel);

    m_genreSortModel=new KNMusicCategorySortFilterModel;
    m_genreSortModel->setFilterKeyColumn(0);
    m_genreSortModel->setFilterCaseSensitivity(Qt::CaseInsensitive);
    m_genreSortModel->setSourceModel(m_genreModel);

    m_albumView=new KNMusicAlbumView(this);
    m_albumView->setCategoryModel(m_albumSortModel);
    m_albumView->installEventFilter(this);
    connect(m_albumView, &KNMusicAlbumView::requireOpenUrl,
            [=](const QModelIndex &index)
            {
                emit requireSetProxy(m_albumDetails);
                emit requirePlayMusic(m_musicModel->filePathFromIndex(m_albumDetails->mapToSource(index)));
            });
    connect(m_albumView, &KNMusicAlbumView::requireShowContextMenu,
            this, &KNMusicViewer::onActionAlbumShowContextMenu);

    m_genreView=new KNMusicCategoryView(this);
    m_genreSongView=new KNMusicGenreSongs(m_genreView);
    m_genreView->installEventFilter(this);
    m_genreSongView->installEventFilter(this);
    m_genreView->setSongListView(m_genreSongView);
    m_genreView->setModel(m_genreSortModel);
    connect(m_genreSongView, &KNMusicGenreSongs::requireOpenUrl,
            [=](const QModelIndex &index)
            {
                emit requireSetProxy(m_genreDetails);
                emit requirePlayMusic(m_musicModel->filePathFromIndex(m_genreDetails->mapToSource(index)));
            });
    connect(m_genreSongView, &KNMusicGenreSongs::requireShowContextMenu,
            this, &KNMusicViewer::onActionGenreShowContextMenu);

    m_playlistView=new KNMusicPlaylistView(this);
    m_playlistView->installEventFilter(this);
    connect(m_playlistView, &KNMusicPlaylistView::requirePlayMusic,
            this, &KNMusicViewer::requirePlayMusic);

    m_albumDetails=new KNMusicAlbumDetailModel;
    m_albumDetails->setFilterKeyColumn(KNMusicGlobal::Album);
    m_albumDetails->setCategoryModel(m_albumModel);
    m_albumView->setDetailModel(m_albumDetails);

    m_genreDetails=new KNMusicCategoryDetailModel;
    m_genreDetails->setFilterKeyColumn(KNMusicGlobal::Genre);
    m_genreDetails->setCategoryModel(m_genreModel);
    m_genreView->setDetailModel(m_genreDetails);

//    addCategory(,
//                m_categoryCaption[Artists],
//                m_artistView);
    addCategory(QPixmap(":/Category/Resources/Category/03_ablums.png"),
                m_categoryCaption[Albums],
                m_albumView);
    addCategory(QPixmap(":/Category/Resources/Category/04_genres.png"),
                m_categoryCaption[Genres],
                m_genreView);
    addCategory(QPixmap(":/Category/Resources/Category/05_playlists.png"),
                m_categoryCaption[Playlists],
                m_playlistView);

    m_playerIn=new QPropertyAnimation(this);
    m_playerIn->setPropertyName("geometry");
    m_playerIn->setEasingCurve(QEasingCurve::OutCubic);

    m_playerOut=new QPropertyAnimation(this);
    m_playerOut->setPropertyName("geometry");
    m_playerOut->setEasingCurve(QEasingCurve::OutCubic);
}

KNMusicViewer::~KNMusicViewer()
{
//    m_listViewModel->deleteLater();
//    m_artistModel->deleteLater();
//    m_artistSortModel->deleteLater();
//    m_artistDetails->deleteLater();
    m_albumModel->deleteLater();
    m_genreModel->deleteLater();
    m_albumSortModel->deleteLater();
    m_genreSortModel->deleteLater();
    m_albumDetails->deleteLater();
    m_genreDetails->deleteLater();
}

void KNMusicViewer::setPlaylistManager(KNMusicPlaylistManager *manager)
{
    m_playlistView->setManager(manager);
}

void KNMusicViewer::setMusicModel(KNMusicLibraryModelBase *model)
{
    emit requireSetMusicModel(model);
    m_albumModel->setSourceModel(model);
    m_albumDetails->setSourceModel(model);
    m_genreModel->setSourceModel(model);
    m_genreDetails->setSourceModel(model);
    m_albumView->setSourceModel(model);
    m_genreSongView->setSourceModel(model);
    m_albumView->resetHeader();
    m_genreView->resetHeader();
    m_musicModel=model;
}

void KNMusicViewer::setBackend(KNMusicBackend *backend)
{
    emit requireSetBackend(backend);
    m_albumView->setMusicBackend(backend);
    m_genreSongView->setMusicBackend(backend);
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
    if(event->type()==QEvent::MouseButtonRelease)
    {
        QMouseEvent *releaseMouse=static_cast<QMouseEvent *>(event);
        //if(releaseMouse)
        switch(releaseMouse->button())
        {
        case Qt::BackButton:
            moveLeft();
            return true;
        case Qt::ForwardButton:
            moveRight();
            return true;
        default:
            return KNStdLibViewer::eventFilter(watched, event);
        }
    }
    return KNStdLibViewer::eventFilter(watched, event);
}

void KNMusicViewer::setPlayWidget(QWidget *widget)
{
    m_playerWidget=widget;
    m_playerWidget->setGeometry(QRect(0, -height(), width(), height()));
    m_playerIn->setTargetObject(m_playerWidget);
    m_playerOut->setTargetObject(m_playerWidget);
}

void KNMusicViewer::addListviewModePlugin(KNMusicViewerItem *plugin)
{
    connect(this, &KNMusicViewer::requireRemoveOriginal,
            plugin, &KNMusicViewerItem::onActionRemoveOriginalItem);
    connect(this, &KNMusicViewer::requireShowInSongsView,
            plugin, &KNMusicViewerItem::onActionShowIndex);
    addPlugin(plugin);
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
    m_albumSortModel->sort(0);
    m_genreSortModel->sort(0);
}

void KNMusicViewer::showIn(const int &category,
                           const QModelIndex &index)
{
    emit requireClearSearch();
    switch(category)
    {
    case KNMusicGlobal::SongsView:
        emit requireShowInSongsView(index);
        break;
    case KNMusicGlobal::ArtistView:
//        emit requireShowInArtistView(index);
//        m_artistView->selectCategoryItem(m_musicModel->data(m_musicModel->index(index.row(),
//                                                                                KNMusicGlobal::Artist)).toString());
//        m_artistView->selectItem(index);
        break;
    case KNMusicGlobal::AlbumView:
        m_albumView->selectCategoryItem(m_musicModel->data(m_musicModel->index(index.row(),
                                                                               KNMusicGlobal::Album)).toString());
        m_albumView->selectItem(index);
        break;
    case KNMusicGlobal::GenreView:
        m_genreView->selectCategoryItem(m_musicModel->data(m_musicModel->index(index.row(),
                                                                               KNMusicGlobal::Genre)).toString());
        m_genreView->selectItem(index);
        break;
    }
    setCategoryIndex(category);
}

void KNMusicViewer::showInCurrent(const QModelIndex &index)
{
    showIn(categoryIndex(), index);
}

void KNMusicViewer::deleteMusic(const QModelIndex &index)
{
    emit requireRemoveMusic(index);
    m_albumModel->onMusicRemoved(index);
    m_genreModel->onMusicRemoved(index);
    m_musicModel->removeAppendData(index);
    emit requireRemoveOriginal(index);
}

void KNMusicViewer::deleteSelections()
{
    QModelIndexList selectionList=KNMusicGlobal::instance()->selectedIndexes();
    QList<int> rowLists;
    while(!selectionList.isEmpty())
    {
        rowLists.append(selectionList.takeFirst().row());
    }
    qStableSort(rowLists.begin(), rowLists.end(), qGreater<int>());
    while(!rowLists.isEmpty())
    {
        deleteMusic(m_musicModel->index(rowLists.takeFirst(), 0));
    }
}

void KNMusicViewer::onActionSearch(const QString &text)
{
    m_albumView->setFilterFixedString(text);
    m_genreSortModel->setFilterFixedString(text);
}

void KNMusicViewer::onActionShowPlayer()
{
    m_playerOut->stop();
    if(m_playerIn->state()==QPropertyAnimation::Running)
    {
        m_playerIn->pause();
    }
    m_playerIn->setStartValue(m_playerWidget->geometry());
    m_playerIn->setEndValue(geometry());
    m_playerIn->start();
}

void KNMusicViewer::onActionHidePlayer()
{
    m_playerIn->stop();
    if(m_playerOut->state()==QPropertyAnimation::Running)
    {
        m_playerOut->pause();
    }
    m_playerOut->setStartValue(m_playerWidget->geometry());
    m_playerOut->setEndValue(QRect(0, -height(), width(), height()));
    m_playerOut->start();
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

void KNMusicViewer::resizeEvent(QResizeEvent *event)
{
    KNStdLibViewer::resizeEvent(event);
    m_playerWidget->setGeometry(m_playerWidget->y()==0?
                            geometry():QRect(0, -height(), width(), height()));
}

void KNMusicViewer::onActionArtistShowContextMenu(const QPoint &position)
{
    emit requireShowContextMenu(position,
                                KNMusicGlobal::ArtistView);
}

void KNMusicViewer::onActionAlbumShowContextMenu(const QPoint &position)
{
    emit requireShowContextMenu(position,
                                KNMusicGlobal::AlbumView);
}

void KNMusicViewer::onActionGenreShowContextMenu(const QPoint &position)
{
    emit requireShowContextMenu(position,
                                KNMusicGlobal::GenreView);
}

void KNMusicViewer::addPlugin(KNMusicViewerItem *plugin)
{
    //Connect actions.
    connect(this, &KNMusicViewer::requireResort,
            plugin, &KNMusicViewerItem::onActionResort);
    connect(this, &KNMusicViewer::requireSearch,
            plugin, &KNMusicViewerItem::onActionSearch);
    connect(this, &KNMusicViewer::requireSetMusicModel,
            plugin, &KNMusicViewerItem::setMusicSourceModel);
    connect(this, &KNMusicViewer::requireSetBackend,
            plugin, &KNMusicViewerItem::setBackend);

    //Connect requires.
    connect(plugin, &KNMusicViewerItem::requirePlayMusic,
            [=](const QModelIndex &sourceIndex)
            {
                emit requirePlayMusic(m_musicModel->filePathFromIndex(sourceIndex));
            });
    connect(plugin, &KNMusicViewerItem::requireShowContextMenu,
            this, &KNMusicViewer::requireShowContextMenu);
    connect(plugin, &KNMusicViewerItem::requireAddCategory,
            this, &KNMusicViewer::addCategory);

    //Apply plugin.
    plugin->applyPlugin();
}
