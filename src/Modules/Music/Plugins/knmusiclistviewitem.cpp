#include "../Libraries/knmusicsortmodel.h"
#include "../Widgets/knmusiclistview.h"

#include "knmusiclistviewitem.h"

KNMusicListviewItem::KNMusicListviewItem(QObject *parent) :
    KNMusicViewerItem(parent)
{
    m_listViewModel=new KNMusicSortModel;
    m_listViewModel->setFilterKeyColumn(-1);
    m_listViewModel->setFilterCaseSensitivity(Qt::CaseInsensitive);
    m_listViewModel->setSortCaseSensitivity(Qt::CaseInsensitive);

    m_libraryView=new KNMusicListView;
    m_libraryView->setModel(m_listViewModel);
    m_libraryView->installEventFilter(this);
    connect(m_libraryView, &KNMusicListView::requireOpenUrl,
            [=](const QModelIndex &index)
            {
                emit requireSetProxy(m_listViewModel);
//                emit requirePlayMusic(m_musicModel->filePathFromIndex(m_listViewModel->mapToSource(index)));
            });
    connect(m_libraryView, &KNMusicListView::requireShowContextMenu,
            this, &KNMusicListviewItem::onActionShowContextMenu);
}

KNMusicListviewItem::~KNMusicListviewItem()
{
    m_libraryView->deleteLater();
    m_listViewModel->deleteLater();
}

void KNMusicListviewItem::setSourceModel(KNMusicLibraryModelBase *model)
{
    m_libraryView->setSourceModel(model);
    m_libraryView->resetHeader();
}

QWidget *KNMusicListviewItem::widget()
{
    return m_libraryView;
}

void KNMusicListviewItem::onActionShowContextMenu(const QPoint &position)
{
    emit requireShowContextMenu(position,
                                KNMusicGlobal::SongsView);
}
