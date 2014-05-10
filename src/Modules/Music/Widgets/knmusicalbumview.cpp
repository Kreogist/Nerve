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
#include "../Libraries/knmusicalbummodel.h"
#include "../Libraries/knmusicalbumdetailmodel.h"

#include "knmusicalbumview.h"

KNMusicAlbumSongDetail::KNMusicAlbumSongDetail(QWidget *parent) :
    QWidget(parent)
{
    setAutoFillBackground(true);
    setContentsMargins(0,0,0,0);

    QPalette pal=palette();
    pal.setColor(QPalette::Base, QColor(0xc0,0xc0,0xc0));
    pal.setColor(QPalette::Window, QColor(0xff,0xff,0xff, 240));
    pal.setColor(QPalette::Text, QColor(0,0,0));
    setPalette(pal);

    m_mainLayout=new QBoxLayout(QBoxLayout::TopToBottom, this);
    m_mainLayout->setContentsMargins(0,0,0,0);
    m_mainLayout->setSpacing(0);
    setLayout(m_mainLayout);

    m_albumName=new QLabel(this);
    m_albumName->setContentsMargins(20,0,0,0);
    QFont artistFont=font();
    artistFont.setPointSize(artistFont.pointSize()+(artistFont.pointSize()>>1));
    artistFont.setBold(true);
    m_albumName->setFont(artistFont);
    m_mainLayout->addSpacing(25);
    m_mainLayout->addWidget(m_albumName);

    m_detailLayout=new QBoxLayout(QBoxLayout::LeftToRight);

    m_artistName=new QLabel(this);
    //This hack is going to show the left margins instead of the layout.
    m_artistName->setContentsMargins(20,0,0,0);
    m_detailLayout->addWidget(m_artistName);
    m_detailLayout->addStretch();

    m_mainLayout->addLayout(m_detailLayout);

    m_albumSongs=new KNMusicAlbumSongListView(this);
    m_mainLayout->addSpacing(14);
    m_mainLayout->addWidget(m_albumSongs, 1);
    connect(m_albumSongs, &KNMusicAlbumSongListView::requireOpenUrl,
            this, &KNMusicAlbumSongDetail::requireOpenUrl);
    connect(m_albumSongs, &KNMusicAlbumSongListView::requireShowContextMenu,
            this, &KNMusicAlbumSongDetail::requireShowContextMenu);
}

KNMusicAlbumSongDetail::~KNMusicAlbumSongDetail()
{
    m_detailLayout->deleteLater();
}

void KNMusicAlbumSongDetail::setAlbumName(const QString &name)
{
    m_albumName->setText(name);
}

void KNMusicAlbumSongDetail::setArtistName(const QString &name)
{
    m_artistName->setText(name);
}

void KNMusicAlbumSongDetail::setDetailModel(KNMusicAlbumDetailModel *model)
{
    m_albumSongs->setModel(model);
}

void KNMusicAlbumSongDetail::selectItem(const QModelIndex &index)
{
    m_albumSongs->setCurrentIndex(index);
}

void KNMusicAlbumSongDetail::resetHeader()
{
    m_albumSongs->resetHeader();
}

void KNMusicAlbumSongDetail::hideDetailInfo()
{
    m_albumName->hide();
    m_artistName->hide();
    m_albumSongs->hide();
}

void KNMusicAlbumSongDetail::showDetailInfo()
{
    m_albumName->show();
    m_artistName->show();
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
    pal.setColor(QPalette::Window, QColor(0xc0,0xc0,0xc0, 240));
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

void KNMusicAlbumInfoDetail::hideDetailInfo()
{
    emit changeInfoVisible(false);
}

void KNMusicAlbumInfoDetail::showDetailInfo()
{
    emit changeInfoVisible(true);
}

void KNMusicAlbumInfoDetail::onActionSongCountChange(const int &value)
{
    m_songCount=value;
    updateSongCount();
}

void KNMusicAlbumInfoDetail::updateSongCount()
{
    if(m_songCount==1)
    {
        m_albumInfo[SongCount]->setText(m_songCountText);
    }
    else
    {
        m_albumInfo[SongCount]->setText(m_songsCountText.arg(m_songCount));
    }
}

int KNMusicAlbumInfoDetail::minimalExpandedHeight() const
{
    return m_minimalExpandedHeight;
}

void KNMusicAlbumInfoDetail::retranslate()
{
    m_songCountText=tr("1 song");
    m_songsCountText=tr("%1 songs");
}

void KNMusicAlbumInfoDetail::retranslateAndSet()
{
    retranslate();
    updateSongCount();
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
    connect(m_songPanel, &KNMusicAlbumSongDetail::requireOpenUrl,
            this, &KNMusicAlbumDetail::requireOpenUrl);
    connect(m_songPanel, &KNMusicAlbumSongDetail::requireShowContextMenu,
            this, &KNMusicAlbumDetail::requireShowContextMenu);

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

void KNMusicAlbumDetail::setArtistName(const QString &name)
{
    m_songPanel->setArtistName(name);
}

void KNMusicAlbumDetail::setDetailModel(KNMusicAlbumDetailModel *model)
{
    m_songPanel->setDetailModel(model);
    connect(model, &KNMusicAlbumDetailModel::requireSongCountChange,
            m_infoPanel, &KNMusicAlbumInfoDetail::onActionSongCountChange);
}

void KNMusicAlbumDetail::selectItem(const QModelIndex &index)
{
    m_songPanel->selectItem(index);
}

void KNMusicAlbumDetail::resetHeader()
{
    m_songPanel->resetHeader();
}

void KNMusicAlbumDetail::expandDetail()
{
    int parentHeight=parentWidget()->height(),
        heightEnd=qMax(height()+m_infoPanel->minimalExpandedHeight(),
                       (parentHeight>>1)),
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
    connect(m_albumDetail, &KNMusicAlbumDetail::requireOpenUrl,
            this, &KNMusicAlbumView::requireOpenUrl);
    connect(m_albumDetail, &KNMusicAlbumDetail::requireShowContextMenu,
            this, &KNMusicAlbumView::requireShowContextMenu);

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

    updateParameters();
}

QModelIndex KNMusicAlbumView::indexAt(const QPoint &point) const
{
    int pointTop=verticalScrollBar()->value()+point.y(),
        spacingHeight=m_gridHeight+m_spacing,
        spacingWidth=m_spacing+m_gridWidth,
        pointLine=pointTop/spacingHeight,
        pointColumn=point.x()/spacingWidth,
        horizontalPosition=point.x()-pointColumn*spacingWidth;
    if(pointTop-pointLine*spacingHeight<m_spacing ||
       horizontalPosition<m_spacing ||
       horizontalPosition>m_spacing+m_iconSizeParam)
    {
        //Clicked on space.
        return QModelIndex();
    }
    int originalRow=pointLine*m_maxColumnCount+pointColumn;
    return m_model->index(m_model->isNoAlbumHidden()?originalRow+1:originalRow,
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

void KNMusicAlbumView::setCategoryModel(KNMusicAlbumModel *model)
{
    setModel(model);
    connect(model, &KNMusicAlbumModel::requireShowFirstItem,
            this, &KNMusicAlbumView::showFirstItem);
    connect(model, &KNMusicAlbumModel::requireHideFirstItem,
            this, &KNMusicAlbumView::hideFirstItem);
    m_model=model;
}

void KNMusicAlbumView::setDetailModel(KNMusicAlbumDetailModel *model)
{
    m_albumDetail->setDetailModel(model);
    m_detailModel=model;
}

void KNMusicAlbumView::selectCategoryItem(const QString &value)
{
    if(value.isEmpty())
    {
        scrollTo(model()->index(0,0));
        selectAlbum(model()->index(0,0));
        return;
    }
    QList<QStandardItem *> albumSearch=m_model->findItems(value);
    if(albumSearch.size()==0)
    {
        return;
    }
    scrollTo(albumSearch.at(0)->index());
    selectAlbum(albumSearch.at(0)->index());
}

void KNMusicAlbumView::selectItem(const QModelIndex &index)
{
    QModelIndex testIndex=m_detailModel->mapFromSource(index);
    if(testIndex.isValid())
    {
        m_albumDetail->selectItem(testIndex);
    }
}

void KNMusicAlbumView::updateGeometries()
{
    int verticalMax=qMax(0,
                         m_lineCount*(m_gridHeight+m_spacing)+m_spacing-height());
    verticalScrollBar()->setRange(0, verticalMax);
    verticalScrollBar()->setSingleStep(m_lineCount==0?10:
                                                      verticalMax/m_lineCount*(height()/(m_spacing+m_gridHeight)));
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

    updateParameters();

    int albumIndex=0, albumCount=m_model->rowCount(),
        currentRow=0, currentColumn=0,
        currentLeft=m_spacing, currentTop=m_spacing;
    m_lineCount=(albumCount+m_maxColumnCount-1)/m_maxColumnCount;

    painter.translate(0, -verticalScrollBar()->value());
    int skipLineCount=verticalScrollBar()->value()/(m_gridHeight+m_spacing),
        drawnHeight=0, maxDrawnHeight=height()+m_gridHeight+m_spacing;
    currentRow+=skipLineCount;
    currentTop+=(m_spacing+m_gridHeight)*skipLineCount;
    albumIndex=skipLineCount*m_maxColumnCount;
    if(m_model->isNoAlbumHidden())
    {
        albumIndex++;
    }
    m_firstVisibleIndex=albumIndex;
    QModelIndex currentPaintIndex;
    while(albumIndex < albumCount && drawnHeight < maxDrawnHeight)
    {
        currentPaintIndex=m_model->index(albumIndex, 0, rootIndex());
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
        m_albumDetail->setGeometry((width()>>2),
                                   (height()>>2),
                                   (width()>>1),
                                   (height()>>1));
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
    if(index.row()==0 && m_model->isNoAlbumHidden())
    {
        return true;
    }
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
     /*if(m_pressedIndex==indexAt(e->pos()) &&
        m_pressedIndex.isValid())
     {
         ;
     }*/
}

void KNMusicAlbumView::onActionAlbumClicked(const QModelIndex &index)
{
    m_albumDetail->hide();
    m_detailIndex=index;
    QIcon currentIcon=m_model->data(index, Qt::DecorationRole).value<QIcon>();
    m_albumDetail->setAlbumArt(currentIcon.pixmap(m_iconSizeParam-2,m_iconSizeParam-2),
                               QSize(m_iconSizeParam-2,m_iconSizeParam-2));
    m_detailModel->setCategoryIndex(index);
    m_albumDetail->setAlbumName(m_model->data(index).toString());
    m_albumDetail->setArtistName(m_model->data(index, Qt::UserRole).toString());
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

void KNMusicAlbumView::showFirstItem()
{
    ;
}

void KNMusicAlbumView::hideFirstItem()
{
    ;
}

QRect KNMusicAlbumView::itemRect(const QModelIndex &index) const
{
    if(!index.isValid())
    {
        return QRect();
    }
    int itemIndex=m_model->isNoAlbumHidden()?index.row()-1:index.row(),
        itemLine=itemIndex/m_maxColumnCount,
        itemColumn=itemIndex-itemLine*m_maxColumnCount;
    return QRect(itemColumn*(m_spacing+m_gridWidth)+m_spacing,
                 itemLine*(m_spacing+m_gridHeight)+m_spacing,
                 m_gridWidth,
                 m_gridHeight);
}

void KNMusicAlbumView::updateParameters()
{
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
    m_iconSizeParam=qMin(m_gridWidth-m_spacing,
                         m_gridHeight-(fontMetrics().height()<<1)-m_spacing);
}

void KNMusicAlbumView::paintAlbum(QPainter *painter,
                                  const QRect &rect,
                                  const QModelIndex &index)
{
    //To draw the album art.
    QIcon currentIcon=m_model->data(index, Qt::DecorationRole).value<QIcon>();
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
                      m_model->data(index).toString());
    textTop+=fontMetrics().height();
    QColor penBackup=painter->pen().color();
    painter->setPen(QColor(128,128,128));
    painter->drawText(rect.x(),
                      textTop,
                      rect.width()-m_spacing,
                      fontMetrics().height(),
                      Qt::TextSingleLine | Qt::AlignLeft | Qt::AlignTop,
                      m_model->data(index, Qt::UserRole).toString());
    painter->setPen(penBackup);
}

int KNMusicAlbumView::gridMinimumWidth() const
{
    return m_gridMinimumWidth;
}

void KNMusicAlbumView::setGridMinimumWidth(int gridMinimumWidth)
{
    m_gridMinimumWidth=gridMinimumWidth;
}

void KNMusicAlbumView::resetHeader()
{
    m_albumDetail->resetHeader();
}

void KNMusicAlbumView::selectAlbum(const QModelIndex &index)
{
    if(index.isValid())
    {
        if(index!=m_detailIndex)
        {
            onActionAlbumClicked(index);
        }
    }
    else
    {
        m_albumDetail->foldDetail();
        selectionModel()->clear();
    }
}
