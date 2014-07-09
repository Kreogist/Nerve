#include "knmusiclibrarylistview.h"

#include <QCursor>
#include <QDrag>
#include <QHelpEvent>
#include <QCloseEvent>
#include <QMimeData>
#include <QModelIndexList>
#include <QMouseEvent>
#include <QList>
#include <QGraphicsDropShadowEffect>
#include <QScopedPointer>
#include <QScrollBar>
#include <QSortFilterProxyModel>
#include <QToolTip>
#include <QUrl>

#include <QDebug>

#include "../../knlocale.h"
#include "../knmusicglobal.h"

#include "knmusiclistviewheader.h"
#include "knmusicdetailtooltip.h"

KNMusicLibraryListview::KNMusicLibraryListview(QWidget *parent) :
    KNMusicListViewBase(parent)
{
    //Set locale.
    retranslate();
    connect(KNLocale::instance(), &KNLocale::languageChanged,
            this, &KNMusicLibraryListview::retranslateAndSet);

    m_mimeData=new QMimeData;
    m_dragAction=new QDrag(this);

    //Set music header.
    m_headerWidget=new KNMusicListViewHeader(this);
    m_headerWidget->installEventFilter(this);
    setHeader(m_headerWidget);
    connect(m_headerWidget, &KNMusicListViewHeader::requireChangeVisible,
            this, &KNMusicLibraryListview::onSectionVisibleChanged);

    //Initial the tooltip widget
    m_musicDetailTooltip=new KNMusicDetailTooltip(this);
    m_musicDetailTooltip->installEventFilter(this);
    connect(this, &KNMusicLibraryListview::activated,
            this, &KNMusicLibraryListview::onItemActived);
}

KNMusicLibraryListview::~KNMusicLibraryListview()
{
    m_dragAction->deleteLater();
}

void KNMusicLibraryListview::resetHeader()
{
    connect(header(), &QHeaderView::sortIndicatorChanged,
            this, &KNMusicLibraryListview::onActionSort);
    KNMusicListViewBase::resetHeader();
}

void KNMusicLibraryListview::setModel(QAbstractItemModel *model)
{
    QTreeView::setModel(model);
    m_proxyModel=static_cast<QSortFilterProxyModel *>(model);
}

void KNMusicLibraryListview::setSourceModel(KNMusicLibraryModelBase *musicModel)
{
    m_musicModel=musicModel;
    m_musicDetailTooltip->setMusicModel(musicModel);
}

void KNMusicLibraryListview::setMusicBackend(KNMusicBackend *backend)
{
    m_musicDetailTooltip->setMusicBackend(backend);
}

void KNMusicLibraryListview::retranslate()
{
    ;
}

void KNMusicLibraryListview::retranslateAndSet()
{
    retranslate();
}

void KNMusicLibraryListview::closeEvent(QCloseEvent *event)
{
    m_musicDetailTooltip->forceQuit();
    QTreeView::closeEvent(event);
}

void KNMusicLibraryListview::mouseReleaseEvent(QMouseEvent *event)
{
    QTreeView::mouseReleaseEvent(event);
    if(event->button()==Qt::RightButton &&
          rect().contains(event->pos()))
    {
        QModelIndex mouseIndex=indexAt(event->pos());
        if(mouseIndex.isValid())
        {
            m_musicDetailTooltip->hide();
            KNMusicGlobal::instance()->setSelectedColumn(mouseIndex.column());
            setSelectedRows();
            emit requireShowContextMenu(event->globalPos());
        }
    }
}

void KNMusicLibraryListview::leaveEvent(QEvent *event)
{
    if(!rect().contains(mapFromGlobal(QCursor::pos())))
    {
        m_musicDetailTooltip->hide();
    }
    QTreeView::leaveEvent(event);
}

void KNMusicLibraryListview::hideEvent(QHideEvent *event)
{
    QTreeView::hideEvent(event);
    m_musicDetailTooltip->hide();
}

void KNMusicLibraryListview::startDrag(Qt::DropActions supportedActions)
{
    Q_UNUSED(supportedActions)
    QModelIndexList indexes=selectedIndexes();
    if(indexes.size()>0)
    {
        QModelIndex currentIndex;
        QList<int> rows;
        for(int i=0, listCount=indexes.size();
            i<listCount;
            i++)
        {
            currentIndex=m_proxyModel->mapToSource(indexes.at(i));
            if(!rows.contains(currentIndex.row()))
            {
                rows.append(currentIndex.row());
            }
        }
        m_mimeData->clear();
        QList<QUrl> fileUrlList;
        for(int i=0, fileCount=rows.size();
            i<fileCount;
            i++)
        {
            fileUrlList.append(QUrl::fromLocalFile(m_musicModel->filePathFromIndex(rows.at(i))));
        }
        m_mimeData->setUrls(fileUrlList);
        m_dragAction->setMimeData(m_mimeData);
        m_dragAction->exec(Qt::CopyAction);
    }
}

bool KNMusicLibraryListview::event(QEvent *event)
{
    if(event->type()==QEvent::ToolTip)
    {
        QHelpEvent *helpEvent=static_cast<QHelpEvent *>(event);
        QPoint realPos=helpEvent->pos();
        realPos.setY(realPos.y()-header()->height());
        QModelIndex index=realPos.y()<0?QModelIndex():indexAt(realPos);
        if(index.isValid())
        {
            if(verticalScrollBar()->isVisible() &&
                (realPos.x()>(viewport()->rect().right()-verticalScrollBar()->width())))
            {
                m_musicDetailTooltip->hide();
            }
            else
            {
                m_musicDetailTooltip->setTooltip(m_proxyModel->mapToSource(index),
                                                 mapToGlobal(helpEvent->pos()));
                m_musicDetailTooltip->showTooltip();
            }
        }
        return true;
    }
    return QTreeView::event(event);
}

void KNMusicLibraryListview::onSectionVisibleChanged(const int &index,
                                              const bool &visible)
{
    setColumnHidden(index, !visible);
}

void KNMusicLibraryListview::onItemActived(const QModelIndex &index)
{
    if(index.isValid())
    {
        emit requireOpenUrl(index);
    }
}

void KNMusicLibraryListview::setSelectedRows()
{
    QModelIndexList currentSelection=selectedIndexes(),
                    mappedSelection;
    QList<int> indexMap;
    int indexCount=currentSelection.size(),
        currentRow;
    while(indexCount--)
    {
        QModelIndex currentIndex=currentSelection.takeFirst();
        currentRow=currentIndex.row();
        if(indexMap.indexOf(currentRow)==-1)
        {
            indexMap.append(currentRow);
            mappedSelection.append(m_proxyModel->mapToSource(currentIndex));
        }
    }
    KNMusicGlobal::instance()->setSelectedIndexes(mappedSelection);
}
