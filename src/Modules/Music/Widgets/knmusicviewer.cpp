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

#include "knmusicplaylistview.h"

#include "../../Base/knlibsearcher.h"

#include "../Plugins/knmusiclistviewitem.h"
#include "../Plugins/knmusicartistviewitem.h"
#include "../Plugins/knmusicalbumviewitem.h"
#include "../Plugins/knmusicgenreviewitem.h"

#include "../Libraries/knmusiclibrarymodel.h"
#include "../Libraries/knmusicsortmodel.h"
#include "../Libraries/knmusiccategorydetailmodel.h"

#include "knmusicviewer.h"

KNMusicViewer::KNMusicViewer(QWidget *parent) :
    KNStdLibViewer(parent)
{
    retranslate();
    setContentsMargins(0,0,0,0);
    setAcceptDrops(true);

    //Prepare show in signal mapper.
    m_showInMapper=new QSignalMapper(this);

    //Add plugins
    addListViewPlugin(new KNMusicListViewItem);
    addArtistViewPlugin(new KNMusicArtistViewItem);
    addAlbumViewPlugin(new KNMusicAlbumViewItem);
    addGenreViewPlugin(new KNMusicGenreViewItem);

    m_playlistView=new KNMusicPlaylistView(this);
    m_playlistView->installEventFilter(this);
    connect(m_playlistView, &KNMusicPlaylistView::requirePlayMusic,
            this, &KNMusicViewer::requirePlayMusic);

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
    ;
}

void KNMusicViewer::setPlaylistManager(KNMusicPlaylistManager *manager)
{
    m_playlistView->setManager(manager);
}

void KNMusicViewer::setMusicModel(KNMusicLibraryModelBase *model)
{
    emit requireSetMusicModel(model);
    m_musicModel=model;
}

void KNMusicViewer::setBackend(KNMusicBackend *backend)
{
    emit requireSetBackend(backend);
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

void KNMusicViewer::addListViewPlugin(KNMusicViewerItem *plugin)
{
    connect(this, &KNMusicViewer::requireRemoveOriginal,
            plugin, &KNMusicViewerItem::onActionRemoveOriginalItem);
    connect(this, &KNMusicViewer::requireShowInSongsView,
            plugin, &KNMusicViewerItem::onActionShowIndex);
    addPlugin(plugin);
}

void KNMusicViewer::addArtistViewPlugin(KNMusicViewerItem *plugin)
{
    connect(this, &KNMusicViewer::requireRemoveMusic,
            plugin, &KNMusicViewerItem::onActionRemoveItem);
    connect(this, &KNMusicViewer::requireShowInArtistView,
            plugin, &KNMusicViewerItem::onActionShowIndex);
    addPlugin(plugin);
}

void KNMusicViewer::addAlbumViewPlugin(KNMusicViewerItem *plugin)
{
    connect(this, &KNMusicViewer::requireRemoveMusic,
            plugin, &KNMusicViewerItem::onActionRemoveItem);
    connect(this, &KNMusicViewer::requireShowInAlbumView,
            plugin, &KNMusicViewerItem::onActionShowIndex);
    addPlugin(plugin);
}

void KNMusicViewer::addGenreViewPlugin(KNMusicViewerItem *plugin)
{
    connect(this, &KNMusicViewer::requireRemoveMusic,
            plugin, &KNMusicViewerItem::onActionRemoveItem);
    connect(this, &KNMusicViewer::requireShowInGenreView,
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
    ;
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
        emit requireShowInArtistView(index);
        break;
    case KNMusicGlobal::AlbumView:
        emit requireShowInAlbumView(index);
        break;
    case KNMusicGlobal::GenreView:
        emit requireShowInGenreView(index);
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
