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
#include <QPropertyAnimation>

#include "knmusicalbumsonglistview.h"

#include "knmusicalbumview.h"

KNMusicAlbumSongDetail::KNMusicAlbumSongDetail(QWidget *parent) :
    QWidget(parent)
{
    setAutoFillBackground(true);
    setContentsMargins(0,0,0,0);

    QPalette pal=palette();
    pal.setColor(QPalette::Base, QColor(0xc0,0xc0,0xc0));
    pal.setColor(QPalette::Window, QColor(0xff,0xff,0xff, 200));
    pal.setColor(QPalette::Text, QColor(0,0,0));
    setPalette(pal);

    m_mainLayout=new QBoxLayout(QBoxLayout::TopToBottom, this);
    m_mainLayout->setContentsMargins(0,0,0,0);
    m_mainLayout->setSpacing(0);
    setLayout(m_mainLayout);

    m_albumName=new QLabel(this);
    m_albumName->setContentsMargins(10,0,0,0);
    QFont artistFont=font();
    artistFont.setPointSize(artistFont.pointSize()+(artistFont.pointSize()>>1));
    artistFont.setBold(true);
    m_albumName->setFont(artistFont);
    m_mainLayout->addWidget(m_albumName);

    m_albumSongs=new KNMusicAlbumSongListView(this);
    m_mainLayout->addWidget(m_albumSongs, 1);
}

void KNMusicAlbumSongDetail::setAlbumName(const QString &name)
{
    m_albumName->setText(name);
}

void KNMusicAlbumSongDetail::hideDetailInfo()
{
    m_albumName->hide();
    m_albumSongs->hide();
}

void KNMusicAlbumSongDetail::showDetailInfo()
{
    m_albumName->show();
    m_albumSongs->show();
}

KNMusicAlbumInfoDetail::KNMusicAlbumInfoDetail(QWidget *parent) :
    QWidget(parent)
{
    retranslate();
    setAutoFillBackground(true);
    setContentsMargins(0,0,0,0);

    QPalette pal=palette();
    pal.setColor(QPalette::Base, QColor(0xc0,0xc0,0xc0));
    pal.setColor(QPalette::Window, QColor(0xc0,0xc0,0xc0, 200));
    pal.setColor(QPalette::Text, QColor(0,0,0));
    setPalette(pal);

    m_albumDataLayout=new QBoxLayout(QBoxLayout::TopToBottom, this);
    m_albumDataLayout->setContentsMargins(0,0,0,0);
    m_albumDataLayout->setSpacing(0);
    setLayout(m_albumDataLayout);

    for(int i=0; i<AlbumInfoDataCount; i++)
    {
        m_albumInfo[i]=new QLabel(this);
        connect(this, &KNMusicAlbumInfoDetail::changeInfoVisible,
                m_albumInfo[i], &QLabel::setVisible);
        m_albumDataLayout->addWidget(m_albumInfo[i]);
    }
    m_albumDataLayout->addStretch();
    m_minimalExpandedHeight=height();
}

void KNMusicAlbumInfoDetail::setCaption(const int &index,
                                        const QString &data)
{
    if(data.isEmpty())
    {
        m_albumInfo[index]->setStatusTip("N/A");
    }
    else
    {
        m_albumInfo[index]->setStatusTip(data);
    }
    refreshCaption(index);
}

void KNMusicAlbumInfoDetail::refreshCaption(const int &index)
{
    m_albumInfo[index]->setText(m_albumInfoCaption[index] +
                                m_albumInfo[index]->statusTip());
}

void KNMusicAlbumInfoDetail::hideDetailInfo()
{
    emit changeInfoVisible(false);
}

void KNMusicAlbumInfoDetail::showDetailInfo()
{
    emit changeInfoVisible(true);
}

int KNMusicAlbumInfoDetail::minimalExpandedHeight() const
{
    return m_minimalExpandedHeight;
}

void KNMusicAlbumInfoDetail::retranslate()
{
    m_albumInfoCaption[SongCount]=tr("Songs: ");
    m_albumInfoCaption[Year]=tr("Year: ");
}

void KNMusicAlbumInfoDetail::retranslateAndSet()
{
    retranslate();
}

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

    m_infoPanel=new KNMusicAlbumInfoDetail(this);
    m_artInfoLayout->addWidget(m_infoPanel, 1);

    m_infoListLayout->addLayout(m_artInfoLayout);

    m_songPanel=new KNMusicAlbumSongDetail(this);
    m_infoListLayout->addWidget(m_songPanel, 1);
    m_infoListLayout->addStretch();

    m_heightExpand=new QPropertyAnimation(this, "geometry", this);
    m_heightExpand->setDuration(125);
    m_widthExpand=new QPropertyAnimation(this, "geometry", this);
    m_widthExpand->setDuration(125);
    m_widthExpand->setEasingCurve(QEasingCurve::OutCubic);
    connect(m_heightExpand, SIGNAL(finished()),
            m_widthExpand, SLOT(start()));
    connect(m_widthExpand, &QPropertyAnimation::finished,
            this, &KNMusicAlbumDetail::showDetailContent);

    m_widthFold=new QPropertyAnimation(this, "geometry", this);
    m_widthFold->setDuration(125);
    m_heightFold=new QPropertyAnimation(this, "geometry", this);
    m_heightFold->setDuration(125);
    m_heightFold->setEasingCurve(QEasingCurve::OutCubic);
    connect(m_widthFold, SIGNAL(finished()),
            m_heightFold, SLOT(start()));
    connect(m_heightFold, SIGNAL(finished()),
            this, SIGNAL(requireFlyBack()));
    connect(m_heightFold, SIGNAL(finished()),
            this, SLOT(hideDetailWidget()));
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
    m_infoPanel->setFixedWidth(size.width());
}

void KNMusicAlbumDetail::hideDetailWidget()
{
    m_infoPanel->hide();
    m_songPanel->hide();
}

void KNMusicAlbumDetail::showDetailWidget()
{
    m_infoPanel->show();
    m_songPanel->show();
}

void KNMusicAlbumDetail::setAlbumName(const QString &name)
{
    m_songPanel->setAlbumName(name);
}

void KNMusicAlbumDetail::expandDetail()
{
    int heightEnd=qMax(height()+m_infoPanel->minimalExpandedHeight(), 0),
        parentHeight=parentWidget()->height(),
        widthEnd=(parentWidget()->width()>>1),
        topEnd=((parentHeight-heightEnd)>>1);
    QRect heightExpandEnd=QRect(x(),
                                topEnd,
                                width(),
                                heightEnd);
    m_heightExpand->setStartValue(geometry());
    m_heightExpand->setEndValue(heightExpandEnd);
    m_widthExpand->setStartValue(heightExpandEnd);
    m_widthExpand->setEndValue(QRect((widthEnd>>1),
                                     topEnd,
                                     widthEnd,
                                     heightEnd));
    hideDetailContent();
    showDetailWidget();
    m_heightExpand->start();
}

void KNMusicAlbumDetail::foldDetail()
{
    int sizeEnd=m_albumArt->width(),
        topEnd=((parentWidget()->height()-sizeEnd)>>1),
        leftEnd=((parentWidget()->width()-sizeEnd)>>1);
    QRect widthFoldEnd=QRect(leftEnd,
                             y(),
                             sizeEnd,
                             height());
    m_widthFold->setStartValue(geometry());
    m_widthFold->setEndValue(widthFoldEnd);
    m_heightFold->setStartValue(widthFoldEnd);
    m_heightFold->setEndValue(QRect(leftEnd,
                                    topEnd,
                                    sizeEnd,
                                    sizeEnd));
    hideDetailContent();
    m_widthFold->start();
}

void KNMusicAlbumDetail::hideDetailContent()
{
    m_infoPanel->hideDetailInfo();
    m_songPanel->hideDetailInfo();
}

void KNMusicAlbumDetail::showDetailContent()
{
    m_infoPanel->showDetailInfo();
    m_songPanel->showDetailInfo();
}

KNMusicAlbumView::KNMusicAlbumView(QWidget *parent) :
    QAbstractItemView(parent)
{
    verticalScrollBar()->setRange(0, 0);
    verticalScrollBar()->setSingleStep(10);

    m_backgroundColor=QColor(m_minGrey, m_minGrey, m_minGrey);
    m_palette=palette();
    m_palette.setColor(QPalette::Base, m_backgroundColor);
    m_palette.setColor(QPalette::Window, QColor(0x30, 0x30, 0x30));
    m_palette.setColor(QPalette::Button, QColor(0x30, 0x30, 0x30));
    m_palette.setColor(QPalette::ButtonText, QColor(0xff, 0xff, 0xff));
    m_palette.setColor(QPalette::Text, QColor(0xff, 0xff, 0xff));
    m_palette.setColor(QPalette::Highlight, QColor(0x60, 0x60, 0x60));
    m_palette.setColor(QPalette::HighlightedText, QColor(0xf7, 0xcf, 0x3d));
    setPalette(m_palette);

    m_albumDetail=new KNMusicAlbumDetail(this);
    m_albumDetail->hide();

    m_albumShow=new QPropertyAnimation(m_albumDetail,
                                       "geometry",
                                       this);
    m_albumShow->setEasingCurve(QEasingCurve::OutCubic);
    connect(m_albumShow, &QPropertyAnimation::finished,
            m_albumDetail, &KNMusicAlbumDetail::expandDetail);
    m_albumHide=new QPropertyAnimation(m_albumDetail,
                                       "geometry",
                                       this);
    m_albumHide->setEasingCurve(QEasingCurve::OutCubic);
    connect(m_albumHide, &QPropertyAnimation::finished,
            this, &KNMusicAlbumView::onActionHideAlbumDetailFinished);
    connect(m_albumDetail, &KNMusicAlbumDetail::requireFlyBack,
            this, &KNMusicAlbumView::onActionHideAlbumDetail);

    m_scrollTimeLine=new QTimeLine(200, this);
    m_scrollTimeLine->setUpdateInterval(5);
    m_scrollTimeLine->setEasingCurve(QEasingCurve::OutCubic);
    connect(m_scrollTimeLine, SIGNAL(frameChanged(int)),
            verticalScrollBar(), SLOT(setValue(int)));
}

QModelIndex KNMusicAlbumView::indexAt(const QPoint &point) const
{
    int pointTop=verticalScrollBar()->value()+point.y(),
        pointLine=
            (pointTop)/(m_gridHeight+m_spacing),
        pointColumn=
            point.x()/(m_spacing+m_gridWidth);
    if(pointTop-pointLine*(m_spacing+m_gridHeight)<m_spacing ||
            point.x()-pointColumn*(m_spacing+m_gridWidth)<m_spacing)
    {
        //Clicked on space.
        return QModelIndex();
    }
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
    m_scrollTimeLine->setFrameRange(verticalScrollBar()->value(),
                                    atTopPosition);
    m_scrollTimeLine->start();
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
    QStyleOptionViewItem option = viewOptions();
    QBrush background = option.palette.base();
    QPen foreground(option.palette.color(QPalette::Text));
    QPainter painter(viewport());
    painter.setRenderHint(QPainter::TextAntialiasing);
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
    m_gridHeight=m_gridWidth+(fontMetrics().height()<<1);
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
    QModelIndex currentPaintIndex;
    while(albumIndex < albumCount && drawnHeight < maxDrawnHeight)
    {
        currentPaintIndex=model()->index(albumIndex, 0, rootIndex());
        QRect currentRect=QRect(currentLeft,
                                currentTop,
                                m_gridWidth,
                                m_gridHeight);
        if(currentPaintIndex!=m_detailIndex)
        {
            paintAlbum(&painter,
                       currentRect,
                       currentPaintIndex);
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

void KNMusicAlbumView::resizeEvent(QResizeEvent *event)
{
    QAbstractItemView::resizeEvent(event);
    if(m_albumDetail->isVisible())
    {
        m_albumDetail->setGeometry(QRect(((width()-m_albumDetail->width())>>1),
                                         ((height()-m_albumDetail->height())>>1),
                                         (width()>>1),
                                         m_albumDetail->height()));
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
    QModelIndex current=currentIndex();
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
}

void KNMusicAlbumView::mouseReleaseEvent(QMouseEvent *e)
{
     QAbstractItemView::mouseReleaseEvent(e);
     selectAlbum(indexAt(e->pos()));
     viewport()->update();
     update();
     /*if(m_pressedIndex==indexAt(e->pos()) &&
        m_pressedIndex.isValid())
     {
         ;
     }*/
}

void KNMusicAlbumView::onActionAlbumClicked(const QModelIndex &index)
{
    m_albumDetail->hide();
    QIcon currentIcon=model()->data(index, Qt::DecorationRole).value<QIcon>();
    m_albumDetail->setAlbumArt(currentIcon.pixmap(m_iconSizeParam-2,m_iconSizeParam-2),
                               QSize(m_iconSizeParam-2,m_iconSizeParam-2));
    m_detailIndex=index;
    m_albumDetail->setAlbumName(model()->data(index).toString());
    QRect startPosition=visualRect(index);
    m_albumDetail->setGeometry(startPosition.x()+2,
                               startPosition.y()+2,
                               m_iconSizeParam-2,
                               m_iconSizeParam-2);
    m_albumShow->setStartValue(m_albumDetail->geometry());
    m_albumShow->setEndValue(QRect(((width()-m_albumDetail->width())>>1),
                                        ((height()-m_albumDetail->height())>>1),
                                        m_albumDetail->width(),
                                        m_albumDetail->height()));
    m_albumDetail->hideDetailWidget();
    m_albumDetail->show();
    m_albumShow->start();
}

void KNMusicAlbumView::onActionHideAlbumDetail()
{
    QRect endPosition=visualRect(m_detailIndex);
    m_albumHide->setStartValue(m_albumDetail->geometry());
    m_albumHide->setEndValue(QRect(endPosition.x()+2,
                                   endPosition.y()+2,
                                   m_iconSizeParam-2,
                                   m_iconSizeParam-2));
    m_albumHide->start();
}

void KNMusicAlbumView::onActionHideAlbumDetailFinished()
{
    m_albumDetail->hide();
    m_detailIndex=QModelIndex();
    viewport()->update();
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
    m_iconSizeParam=qMin(rect.width()-m_spacing,
                         rect.height()-(fontMetrics().height()<<1)-m_spacing);
    QRect albumArtRect=QRect(rect.x()+1,
                             rect.y()+1,
                             m_iconSizeParam-2,
                             m_iconSizeParam-2);
    painter->drawPixmap(albumArtRect,
                        currentIcon.pixmap(m_iconSizeParam, m_iconSizeParam));

    //To draw the text.
    int textTop=rect.y()+m_iconSizeParam+5;
    painter->drawText(rect.x(),
                      textTop,
                      rect.width()-m_spacing,
                      fontMetrics().height(),
                      Qt::TextSingleLine | Qt::AlignLeft | Qt::AlignTop,
                      model()->data(index).toString());
    textTop+=fontMetrics().height();
    QColor penBackup=painter->pen().color();
    painter->setPen(QColor(128,128,128));
    painter->drawText(rect.x(),
                      textTop,
                      rect.width()-m_spacing,
                      fontMetrics().height(),
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

void KNMusicAlbumView::selectAlbum(const QModelIndex &index)
{
    m_pressedIndex=index;
    if(m_pressedIndex.isValid())
    {
        if(m_pressedIndex!=m_detailIndex)
        {
            onActionAlbumClicked(m_pressedIndex);
        }
    }
    else
    {
        m_albumDetail->foldDetail();
        selectionModel()->clear();
    }
}
