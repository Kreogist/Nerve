#include <QDebug>

#include <QScrollBar>
#include <QPen>
#include <QMouseEvent>
#include <QIcon>
#include <QItemSelectionModel>
#include <QPainter>
#include <QLabel>
#include <QTimeLine>
#include <QPaintEvent>
#include <QBoxLayout>
#include <QAbstractItemView>

#include "knmusicalbumview.h"

KNMusicAlbumDetail::KNMusicAlbumDetail(QWidget *parent) :
    QWidget(parent)
{
    //Set properties.
    setContentsMargins(0,0,0,0);

    m_infoListLayout=new QBoxLayout(QBoxLayout::LeftToRight, this);
    m_infoListLayout->setContentsMargins(0,0,0,0);
    m_infoListLayout->setSpacing(0);
    setLayout(m_infoListLayout);

    m_artInfoLayout=new QBoxLayout(QBoxLayout::TopToBottom);
    m_artInfoLayout->setContentsMargins(0,0,0,0);
    m_artInfoLayout->setSpacing(0);

    m_albumArt=new QLabel(this);
    m_albumArt->setScaledContents(true);
    m_artInfoLayout->addWidget(m_albumArt);
    m_artInfoLayout->addStretch();

    m_infoListLayout->addLayout(m_artInfoLayout);
    m_infoListLayout->addStretch();
}

KNMusicAlbumDetail::~KNMusicAlbumDetail()
{
    m_artInfoLayout->deleteLater();
}

void KNMusicAlbumDetail::setAlbumArt(const QPixmap &pixmap,
                                     const QSize &size)
{
    m_albumArt->setFixedSize(size);
    m_albumArt->setPixmap(pixmap);
}

KNMusicAlbumView::KNMusicAlbumView(QWidget *parent) :
    QAbstractItemView(parent)
{
    setAutoFillBackground(true);
    verticalScrollBar()->setRange(0, 0);
    verticalScrollBar()->setSingleStep(10);

    QPalette pal=palette();
    setPalette(pal);

    m_albumDetail=new KNMusicAlbumDetail(this);
    connect(this, SIGNAL(clicked(QModelIndex)),
            this, SLOT(onActionAlbumClicked(QModelIndex)));

    m_scrollTimeLine=new QTimeLine(100, this);
    m_scrollTimeLine->setUpdateInterval(5);
    m_scrollTimeLine->setEasingCurve(QEasingCurve::OutCubic);
    connect(m_scrollTimeLine, SIGNAL(frameChanged(int)),
            verticalScrollBar(), SLOT(setValue(int)));
}

QModelIndex KNMusicAlbumView::indexAt(const QPoint &point) const
{
    int pointLine=
            (verticalScrollBar()->value()+point.y())/(m_gridHeight+m_spacing),
        pointColumn=
            point.x()/(m_spacing+m_gridWidth);
    return model()->index(pointLine*m_maxColumnCount+pointColumn,
                          0,
                          rootIndex());
}

void KNMusicAlbumView::scrollTo(const QModelIndex &index,
                                QAbstractItemView::ScrollHint hint)
{
    if(!index.isValid())
    {
        return;
    }
    int atTopPosition=index.row()/m_maxColumnCount*(m_gridHeight+m_spacing);
    switch(hint)
    {
    case QAbstractItemView::PositionAtTop:
        break;
    case QAbstractItemView::PositionAtCenter:
        atTopPosition-=(height()-m_gridHeight-m_spacing)>>1;
        break;
    case QAbstractItemView::PositionAtBottom:
        atTopPosition-=height()-m_gridHeight-m_spacing;
        break;
    case QAbstractItemView::EnsureVisible:
        if(rect().contains(visualRect(index), true))
        {
            return;
        }
        int bottomValue=atTopPosition-height()+m_gridHeight+m_spacing,
            toTop=qAbs(verticalScrollBar()->value()-atTopPosition),
            toBottom=qAbs(verticalScrollBar()->value()-bottomValue);
        if(toBottom<toTop)
        {
            atTopPosition=bottomValue;
        }
        break;
    }
    //verticalScrollBar()->setValue(atTopPosition);
    /*m_scrollTimeLine->setFrameRange(verticalScrollBar()->value(),
                                    atTopPosition);
    m_scrollTimeLine->start();*/
    update();
}

QRect KNMusicAlbumView::visualRect(const QModelIndex &index) const
{
    QRect rect=itemRect(index);
    if(rect.isValid())
    {
        return QRect(rect.left() - horizontalScrollBar()->value(),
                     rect.top() - verticalScrollBar()->value(),
                     rect.width(),
                     rect.height());
    }
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
    painter.setRenderHint(QPainter::SmoothPixmapTransform);
    QRect rect=event->rect();
    painter.setRenderHint(QPainter::Antialiasing);
    if(autoFillBackground())
    {
        painter.fillRect(event->rect(), background);
    }
    painter.setPen(foreground);

    int realWidth=width()-m_spacing,
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
    m_firstVisibleIndex=albumIndex;
    while(albumIndex < albumCount && drawnHeight < maxDrawnHeight)
    {
        QModelIndex index=model()->index(albumIndex, 0, rootIndex());
        QRect currentRect=QRect(currentLeft,
                                currentTop,
                                m_gridWidth,
                                m_gridHeight);
        if(!selections->isSelected(index))
        {
            paintAlbum(&painter,
                       currentRect,
                       index);
        }
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
    updateGeometries();
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

void KNMusicAlbumView::setSelection(const QRect &rect,
                                    QItemSelectionModel::SelectionFlags command)
{
    QModelIndex posIndex=indexAt(QPoint(rect.x(),rect.y()));
    selectionModel()->select(posIndex, command);
}

QRegion KNMusicAlbumView::visualRegionForSelection(const QItemSelection &selection) const
{
    if(selection.size()==0)
    {
        return QRect();
    }
    QItemSelectionRange range = selection.at(0);
    QRegion region;
    QRect tst=visualRect(indexAt(QPoint(range.top(), range.left())));
    region+=tst;
    return region;
}

void KNMusicAlbumView::mousePressEvent(QMouseEvent *e)
{
    QAbstractItemView::mousePressEvent(e);
    //QPoint pos=e->pos();
    m_pressedIndex=indexAt(e->pos());
    /*QItemSelectionModel::SelectionFlags command=selectionCommand(m_pressedIndex,
                                                                 e);
    QRect rect(pos, pos);
    setSelection(rect, command);*/
}

void KNMusicAlbumView::mouseReleaseEvent(QMouseEvent *e)
{
     QAbstractItemView::mouseReleaseEvent(e);
     if(m_pressedIndex==indexAt(e->pos()) &&
        m_pressedIndex.isValid())
     {
         ;
     }
}

void KNMusicAlbumView::onActionAlbumClicked(const QModelIndex &index)
{
    QRect startPosition=visualRect(index);
    m_albumDetail->setGeometry(startPosition.x()+2,
                               startPosition.y()+2,
                               startPosition.width(),
                               startPosition.height());
    QIcon currentIcon=model()->data(index, Qt::DecorationRole).value<QIcon>();
    m_albumDetail->setAlbumArt(currentIcon.pixmap(m_iconSizeParam-1,m_iconSizeParam-1),
                               QSize(m_iconSizeParam-1,m_iconSizeParam-1));
}

QRect KNMusicAlbumView::itemRect(const QModelIndex &index) const
{
    if(!index.isValid())
    {
        return QRect();
    }
    int itemLine=index.row()/m_maxColumnCount,
        itemColumn=index.row()-itemLine*m_maxColumnCount;
    return QRect(itemColumn*(m_spacing+m_gridWidth)+m_spacing,
                 itemLine*(m_spacing+m_gridHeight)+m_spacing,
                 m_gridWidth,
                 m_gridHeight);
}

void KNMusicAlbumView::paintAlbum(QPainter *painter,
                                  const QRect &rect,
                                  const QModelIndex &index)
{
    //To draw the album art.
    QIcon currentIcon=model()->data(index, Qt::DecorationRole).value<QIcon>();
    m_iconSizeParam=qMin(rect.width(), rect.height());
    QRect albumArtRect=QRect(rect.x()+1,
                             rect.y()+1,
                             m_iconSizeParam-2,
                             m_iconSizeParam-2);
    painter->drawPixmap(albumArtRect,
                        currentIcon.pixmap(m_iconSizeParam, m_iconSizeParam));
    painter->drawRect(albumArtRect);

    //To draw the text.
    int textTop=rect.y()+m_iconSizeParam+5;
    painter->drawText(rect.x(),
                      textTop,
                      rect.width(),
                      rect.height(),
                      Qt::TextSingleLine | Qt::AlignLeft | Qt::AlignTop,
                      model()->data(index).toString());
    textTop+=fontMetrics().height();
    QColor penBackup=painter->pen().color();
    painter->setPen(QColor(128,128,128));
    painter->drawText(rect.x(),
                      textTop,
                      rect.width(),
                      rect.height(),
                      Qt::TextSingleLine | Qt::AlignLeft | Qt::AlignTop,
                      model()->data(index, Qt::UserRole).toString());
    painter->setPen(penBackup);
}

int KNMusicAlbumView::gridMinimumWidth() const
{
    return m_gridMinimumWidth;
}

void KNMusicAlbumView::setGridMinimumWidth(int gridMinimumWidth)
{
    m_gridMinimumWidth = gridMinimumWidth;
}
