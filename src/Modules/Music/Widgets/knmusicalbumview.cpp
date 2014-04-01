#include <QDebug>

#include <QScrollBar>
#include <QPen>
#include <QPainter>
#include <QPaintEvent>

#include "knmusicalbumview.h"

KNMusicAlbumView::KNMusicAlbumView(QWidget *parent) :
    QAbstractItemView(parent)
{
    horizontalScrollBar()->setRange(0 ,0);
    verticalScrollBar()->setRange(0, 0);
}

QModelIndex KNMusicAlbumView::indexAt(const QPoint &point) const
{
    return QModelIndex();
}

void KNMusicAlbumView::scrollTo(const QModelIndex &index,
                                QAbstractItemView::ScrollHint hint)
{
    ;
}

QRect KNMusicAlbumView::visualRect(const QModelIndex &index) const
{
    QRect rect;
    return rect;
}

void KNMusicAlbumView::dataChanged(const QModelIndex &topLeft,
                                   const QModelIndex &bottomRight)
{
    QAbstractItemView::dataChanged(topLeft, bottomRight);
    viewport()->update();
}

void KNMusicAlbumView::paintEvent(QPaintEvent *event)
{
    QItemSelectionModel *selections = selectionModel();
    QStyleOptionViewItem option = viewOptions();
    QBrush background = option.palette.base();
    QPen foreground(option.palette.color(QPalette::WindowText));
    QPainter painter(viewport());
    QRect rect=event->rect();
    painter.setRenderHint(QPainter::Antialiasing);
    if(autoFillBackground())
    {
        painter.fillRect(event->rect(), background);
    }
    painter.setPen(foreground);

    int realWidth=rect.width()-m_spacing;
    int realMinimumWidth=m_gridMinimumWidth+m_spacing;
    int columns=realWidth/realMinimumWidth;
    m_gridWidth=columns==0?realMinimumWidth:realWidth/columns;
    int currentX=0, currentY=0;
    for(int i=0; i<columns; i++)
    {
        painter.drawRect(currentX, currentY, m_gridWidth, m_gridHeight);
        currentX+=m_gridWidth;
    }
}

int KNMusicAlbumView::horizontalOffset() const
{
    return horizontalScrollBar()->value();
}

int KNMusicAlbumView::verticalOffset() const
{
    return verticalScrollBar()->value();
}

bool KNMusicAlbumView::isIndexHidden(const QModelIndex &index) const
{
    return false;
}

QModelIndex KNMusicAlbumView::moveCursor(QAbstractItemView::CursorAction cursorAction,
                                         Qt::KeyboardModifiers)
{
    QModelIndex current = currentIndex();
    viewport()->update();
    return current;
}

void KNMusicAlbumView::setSelection(const QRect &, QItemSelectionModel::SelectionFlags command)
{
    ;
}

QRegion KNMusicAlbumView::visualRegionForSelection(const QItemSelection &selection) const
{
    QRegion region;
    return region;
}
int KNMusicAlbumView::gridMinimumWidth() const
{
    return m_gridMinimumWidth;
}

void KNMusicAlbumView::setGridMinimumWidth(int gridMinimumWidth)
{
    m_gridMinimumWidth = gridMinimumWidth;
}

