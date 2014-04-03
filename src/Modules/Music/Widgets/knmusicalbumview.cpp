#include <QDebug>

#include <QScrollBar>
#include <QPen>
#include <QIcon>
#include <QPainter>
#include <QPaintEvent>

#include "knmusicalbumview.h"

KNMusicAlbumView::KNMusicAlbumView(QWidget *parent) :
    QAbstractItemView(parent)
{
    verticalScrollBar()->setRange(0, 0);
    horizontalScrollBar()->setPageStep(4);
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

void KNMusicAlbumView::setModel(QAbstractItemModel *model)
{
    QAbstractItemView::setModel(model);
    updateGeometries();
}

void KNMusicAlbumView::dataChanged(const QModelIndex &topLeft,
                                   const QModelIndex &bottomRight)
{
    QAbstractItemView::dataChanged(topLeft, bottomRight);
    viewport()->update();
}

void KNMusicAlbumView::updateGeometries()
{
    verticalScrollBar()->setRange(0, qMax(0,
                                          m_lineCount*(m_gridHeight+m_spacing)+m_spacing-height()));
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

    int realWidth=rect.width()-m_spacing,
        realMinimumWidth=m_gridMinimumWidth+m_spacing;
    if(realWidth<realMinimumWidth)
    {
        m_maxColumnCount=1;
        m_gridWidth=realWidth-m_spacing;
    }
    else
    {
        m_maxColumnCount=realWidth/realMinimumWidth;
        m_gridWidth=realWidth/m_maxColumnCount-m_spacing;
    }
    int albumIndex=0, albumCount=model()->rowCount(),
            currentRow=0, currentColumn=0,
            currentLeft=m_spacing, currentTop=m_spacing;
    m_lineCount=(albumCount+m_maxColumnCount-1)/m_maxColumnCount;

    painter.translate(0, -verticalScrollBar()->value());
    int skipLineCount=verticalScrollBar()->value()/(m_gridHeight+m_spacing),
        drawnHeight=0, maxDrawnHeight=height()+m_gridHeight+m_spacing;
    currentRow+=skipLineCount;
    currentTop+=(m_spacing+m_gridHeight)*skipLineCount;
    albumIndex=skipLineCount*m_maxColumnCount;
    while(albumIndex < albumCount && drawnHeight < maxDrawnHeight)
    {
        QModelIndex index=model()->index(albumIndex, 0, rootIndex());
        if(selections->isSelected(index))
        {
            qDebug()<<"Selected";
        }
        QRect currentRect=QRect(currentLeft,
                                currentTop,
                                m_gridWidth,
                                m_gridHeight);
        paintAlbum(&painter,
                   currentRect,
                   index);
        currentColumn++;
        if(currentColumn==m_maxColumnCount)
        {
            currentColumn=0;
            currentRow++;
            currentLeft=m_spacing;
            currentTop+=m_spacing+m_gridHeight;
            drawnHeight+=m_spacing+m_gridHeight;
        }
        else
        {
            currentLeft+=m_spacing+m_gridWidth;
        }
        albumIndex++;
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

void KNMusicAlbumView::paintAlbum(QPainter *painter,
                                  const QRect &rect,
                                  const QModelIndex &index)
{
    //To draw the text.
    painter->drawText(rect.x(),
                      rect.y(),
                      model()->data(index).toString());
    //To draw the album art.
    QIcon currentIcon=model()->data(index, Qt::DecorationRole).value<QIcon>();
    int sizeParam=qMin(rect.width(), rect.height());
    painter->drawPixmap(QRect(rect.x(),rect.y(),sizeParam,sizeParam),
                        currentIcon.pixmap(sizeParam, sizeParam));
}

int KNMusicAlbumView::gridMinimumWidth() const
{
    return m_gridMinimumWidth;
}

void KNMusicAlbumView::setGridMinimumWidth(int gridMinimumWidth)
{
    m_gridMinimumWidth = gridMinimumWidth;
}

