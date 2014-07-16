#include <QBoxLayout>
#include <QIcon>
#include <QItemSelectionModel>
#include <QMouseEvent>
#include <QPainter>
#include <QPaintEvent>
#include <QPen>
#include <QFontMetrics>
#include <QResizeEvent>
#include <QPropertyAnimation>
#include <QParallelAnimationGroup>
#include <QScrollBar>
#include <QTimeLine>

#include <QDebug>

#include "knmusicalbumsonglistview.h"
#include "../Libraries/knmusicalbummodel.h"
#include "../Libraries/knmusiccategorysortfiltermodel.h"
#include "../Libraries/knmusicalbumdetailmodel.h"
#include "../../Base/knsideshadows.h"
#include "../../Base/knscrolllabel.h"

#include "knmusicalbumview.h"

KNMusicAlbumArtwork::KNMusicAlbumArtwork(QWidget *parent) :
    QLabel(parent)
{
    ;
}

void KNMusicAlbumArtwork::enterEvent(QEvent *event)
{
    QLabel::enterEvent(event);
    emit requireShowArtwork();
}

void KNMusicAlbumArtwork::leaveEvent(QEvent *event)
{
    QLabel::leaveEvent(event);
    emit requireHideArtwork();
}

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

    m_albumCaption=new KNScrollLabel(this);
    m_albumCaption->setContentsMargins(20,0,0,0);
    QFont artistFont=font();
    artistFont.setBold(true);
    artistFont.setPixelSize(artistFont.pixelSize()+(artistFont.pixelSize()>>1));
    m_albumCaption->setFont(artistFont);
    m_mainLayout->addSpacing(25);
    m_mainLayout->addWidget(m_albumCaption);

    m_detailLayout=new QBoxLayout(QBoxLayout::LeftToRight);

    //Set the artist
    m_artistYearLayout=new QBoxLayout(QBoxLayout::LeftToRight,
                                      m_detailLayout->widget());
    //Hack here, instead of the artist caption hack.
    m_artistYearLayout->setContentsMargins(20,0,0,0);
    m_artistYearLayout->setSpacing(5);

    m_artistCaption=new QLabel(this);
    m_artistCaption->setMinimumWidth(0);
    m_artistYearLayout->addWidget(m_artistCaption);
    m_yearCaption=new QLabel(this);
    m_yearCaption->setMinimumWidth(0);
    m_artistYearLayout->addWidget(m_yearCaption);
    m_artistYearLayout->addStretch();
    m_detailLayout->addLayout(m_artistYearLayout);
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

void KNMusicAlbumSongDetail::setAlbumText(const QString &name)
{
    m_albumCaption->setText(name);
}

void KNMusicAlbumSongDetail::setArtistText(const QString &name)
{
    m_artistCaption->setText(name);
}

void KNMusicAlbumSongDetail::setYearText(const QString &name)
{
    m_yearCaption->setText(name.isEmpty()?
                               "":
                               "("+name+")");
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

void KNMusicAlbumSongDetail::resetSongState()
{
    m_albumSongs->verticalScrollBar()->setValue(0);
    m_albumSongs->horizontalScrollBar()->setValue(0);
    m_albumSongs->resizeHeader();
}

void KNMusicAlbumSongDetail::setMusicBackend(KNMusicBackend *backend)
{
    m_albumSongs->setMusicBackend(backend);
}

void KNMusicAlbumSongDetail::setSourceModel(KNMusicLibraryModelBase *model)
{
    m_albumSongs->setSourceModel(model);
}

void KNMusicAlbumSongDetail::hideDetailInfo()
{
    setDetailInfoVisible(false);
}

void KNMusicAlbumSongDetail::showDetailInfo()
{
    setDetailInfoVisible(true);
}

void KNMusicAlbumSongDetail::setDetailInfoVisible(bool visible)
{
    m_albumCaption->setVisible(visible);
    m_artistCaption->setVisible(visible);
    m_albumSongs->setVisible(visible);
    m_yearCaption->setVisible(visible);
}

KNMusicAlbumDetail::KNMusicAlbumDetail(QWidget *parent) :
    QWidget(parent)
{
    //Set properties.
    setContentsMargins(0,0,0,0);

    m_songPanel=new KNMusicAlbumSongDetail(this);
    m_leftShadow=new KNLeftSideShadow(this);
    connect(m_songPanel, &KNMusicAlbumSongDetail::requireOpenUrl,
            this, &KNMusicAlbumDetail::requireOpenUrl);
    connect(m_songPanel, &KNMusicAlbumSongDetail::requireShowContextMenu,
            this, &KNMusicAlbumDetail::requireShowContextMenu);

    m_albumArt=new KNMusicAlbumArtwork(this);
    m_albumArt->setScaledContents(true);
    m_rightShadow=new KNRightSideShadow(this);
    connect(m_albumArt, &KNMusicAlbumArtwork::requireShowArtwork,
            this, &KNMusicAlbumDetail::showArtwork);
    connect(m_albumArt, &KNMusicAlbumArtwork::requireHideArtwork,
            this, &KNMusicAlbumDetail::hideArtwork);

    m_showExpand=new QPropertyAnimation(this, "geometry", this);
    m_showExpand->setDuration(125);
    m_showShrink=new QPropertyAnimation(this, "geometry", this);
    m_showShrink->setDuration(125);
    m_showShrink->setEasingCurve(QEasingCurve::OutCubic);
    connect(m_showExpand, SIGNAL(finished()),
            m_showShrink, SLOT(start()));
    connect(m_showExpand, &QPropertyAnimation::finished,
            this, &KNMusicAlbumDetail::raisePanel);
    connect(m_showShrink, &QPropertyAnimation::finished,
            this, &KNMusicAlbumDetail::showDetailContent);

    m_hideShrink=new QPropertyAnimation(this, "geometry", this);
    m_hideShrink->setDuration(125);
    m_hideShrink->setEasingCurve(QEasingCurve::OutCubic);
    m_hideExpand=new QPropertyAnimation(this, "geometry", this);
    m_hideExpand->setDuration(125);
    m_hideExpand->setEasingCurve(QEasingCurve::OutCubic);
    connect(m_hideShrink, SIGNAL(finished()),
            m_hideExpand, SLOT(start()));
    connect(m_hideShrink, &QPropertyAnimation::finished,
            this, &KNMusicAlbumDetail::raiseArtwork);
    connect(m_hideExpand, &QPropertyAnimation::finished,
            this, &KNMusicAlbumDetail::hideDetailWidget);
    connect(m_hideExpand, &QPropertyAnimation::finished,
            this, &KNMusicAlbumDetail::requireFlyBack);

    m_flyOut=new QPropertyAnimation(this, "geometry", this);
    m_flyOut->setDuration(125);
    m_flyOut->setEasingCurve(QEasingCurve::OutCubic);
    connect(m_flyOut, &QPropertyAnimation::finished,
            this, &KNMusicAlbumDetail::hideDetailWidget);
    connect(m_flyOut, &QPropertyAnimation::finished,
            this, &KNMusicAlbumDetail::requireFlyOut);

    m_coverExpand=new QPropertyAnimation(this, "geometry", this);
    m_coverExpand->setDuration(125);
    m_coverExpand->setEasingCurve(QEasingCurve::OutCubic);

    m_coverShrink=new QPropertyAnimation(this, "geometry", this);
    m_coverShrink->setDuration(125);
    m_coverShrink->setEasingCurve(QEasingCurve::OutCubic);
}

KNMusicAlbumDetail::~KNMusicAlbumDetail()
{
    ;
}

void KNMusicAlbumDetail::setAlbumArt(const QPixmap &pixmap,
                                     const QSize &size)
{
    m_albumArt->resize(size);
    m_albumArt->setPixmap(pixmap);
}

void KNMusicAlbumDetail::hideDetailWidget()
{
    m_songPanel->hide();
}

void KNMusicAlbumDetail::showDetailWidget()
{
    m_songPanel->show();
}

void KNMusicAlbumDetail::setAlbumText(const QString &name)
{
    m_songPanel->setAlbumText(name);
}

void KNMusicAlbumDetail::setArtistText(const QString &name)
{
    m_songPanel->setArtistText(name);
}

void KNMusicAlbumDetail::setYearText(const QString &value)
{
    m_songPanel->setYearText(value);
}

void KNMusicAlbumDetail::setDetailModel(KNMusicAlbumDetailModel *model)
{
    m_songPanel->setDetailModel(model);
    /*connect(model, &KNMusicAlbumDetailModel::requireSongCountChange,
            m_infoPanel, &KNMusicAlbumInfoDetail::onActionSongCountChange);*/
}

void KNMusicAlbumDetail::setSourceModel(KNMusicLibraryModelBase *model)
{
    m_songPanel->setSourceModel(model);
}

void KNMusicAlbumDetail::selectItem(const QModelIndex &index)
{
    m_songPanel->selectItem(index);
}

void KNMusicAlbumDetail::resetHeader()
{
    m_songPanel->resetHeader();
}

void KNMusicAlbumDetail::resetSongState()
{
    m_songPanel->resetSongState();
    m_albumArt->raise();
}

void KNMusicAlbumDetail::disableArtworkExpand()
{
    m_albumArtExpanding=false;
}

void KNMusicAlbumDetail::setMusicBackend(KNMusicBackend *backend)
{
    m_songPanel->setMusicBackend(backend);
}

void KNMusicAlbumDetail::expandDetail()
{
    int parentWidth=parentWidget()->width(),
        heightEnd=height(),
        widthExpand=(heightEnd<<1),
        widthEnd=(heightEnd+(heightEnd>>1));
    QRect widthExpandEnd=QRect(((parentWidth-widthExpand)>>1),
                                y(),
                                widthExpand,
                                height());
    m_showExpand->setStartValue(geometry());
    m_showExpand->setEndValue(widthExpandEnd);
    m_showShrink->setStartValue(widthExpandEnd);
    m_showShrink->setEndValue(QRect(((parentWidth-widthEnd)>>1),
                                     y(),
                                     widthEnd,
                                     height()));
    hideDetailContent();
    showDetailWidget();
    m_showExpand->start();
}

void KNMusicAlbumDetail::foldDetail()
{
    int parentWidth=parentWidget()->width(),
        heightEnd=height(),
        widthExpand=(heightEnd<<1);
    QRect widthFoldEnd=QRect(((parentWidth-widthExpand)>>1),
                             y(),
                             widthExpand,
                             height());
    m_hideShrink->setStartValue(geometry());
    m_hideShrink->setEndValue(widthFoldEnd);
    m_hideExpand->setStartValue(widthFoldEnd);
    m_hideExpand->setEndValue(QRect((parentWidth-heightEnd)>>1,
                                    y(),
                                    heightEnd,
                                    height()));
    hideDetailContent();
    m_hideShrink->start();
}

void KNMusicAlbumDetail::flyAway()
{
    int sizeEnd=height(),
        topEnd=((parentWidget()->height()-sizeEnd)>>1),
        leftEnd=((parentWidget()->width()-sizeEnd)>>1);
    raiseArtwork();
    m_flyOut->setStartValue(geometry());
    m_flyOut->setEndValue(QRect(leftEnd,
                                topEnd,
                                sizeEnd,
                                sizeEnd));
    hideDetailContent();
    m_flyOut->start();
}

void KNMusicAlbumDetail::resizeEvent(QResizeEvent *event)
{
    QWidget::resizeEvent(event);
    int albumArtParam=qMin(event->size().height(),
                           event->size().width());
    m_albumArt->resize(albumArtParam, albumArtParam);
    m_rightShadow->setGeometry(albumArtParam,
                               0,
                               m_shadowWidth,
                               albumArtParam);
    m_songPanel->setGeometry(event->size().width()-albumArtParam,
                             m_songPanel->y(),
                             albumArtParam,
                             albumArtParam);
    m_leftShadow->setGeometry(event->size().width()-albumArtParam-m_shadowWidth,
                              0,
                              m_shadowWidth,
                              albumArtParam);
}

void KNMusicAlbumDetail::hideDetailContent()
{
    m_songPanel->hideDetailInfo();
    m_albumArtExpanding=false;
}

void KNMusicAlbumDetail::showDetailContent()
{
    m_songPanel->showDetailInfo();
    m_albumArtExpanding=true;
}

void KNMusicAlbumDetail::showArtwork()
{
    if(m_albumArtExpanding)
    {
        m_coverShrink->stop();
        m_coverExpand->setStartValue(geometry());
        int finalWidth=height()<<1;
        m_coverExpand->setEndValue(QRect((parentWidget()->width()-finalWidth)>>1,
                                         y(),
                                         finalWidth,
                                         height()));
        m_coverExpand->start();
    }
}

void KNMusicAlbumDetail::hideArtwork()
{
    if(m_albumArtExpanding)
    {
        m_coverExpand->stop();
        m_coverShrink->setStartValue(geometry());
        int finalWidth=(height()>>1)*3;
        m_coverShrink->setEndValue(QRect((parentWidget()->width()-finalWidth)>>1,
                                         y(),
                                         finalWidth,
                                         height()));
        m_coverShrink->start();
    }
}

void KNMusicAlbumDetail::raiseArtwork()
{
    m_albumArt->raise();
    m_rightShadow->raise();
}

void KNMusicAlbumDetail::raisePanel()
{
    m_songPanel->raise();
    m_leftShadow->raise();
}

KNMusicAlbumView::KNMusicAlbumView(QWidget *parent) :
    QAbstractItemView(parent)
{
    //Set default scrollbar properties.
    verticalScrollBar()->setRange(0, 0);

    //Set palette properties.
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
    m_albumShow->setDuration(150);
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

    m_flyawayGroup=new QParallelAnimationGroup(this);
    m_albumThrow=new QPropertyAnimation(m_albumDetail,
                                        "geometry",
                                        this);
    m_albumThrow->setEasingCurve(QEasingCurve::OutCubic);
    m_flyawayGroup->addAnimation(m_albumThrow);
    connect(m_albumDetail, &KNMusicAlbumDetail::requireFlyOut,
            this, &KNMusicAlbumView::onActionFlyAwayAlbumDetail);
    connect(m_flyawayGroup, &QParallelAnimationGroup::finished,
            this, &KNMusicAlbumView::onActionFlyAwayAlbumDetailFinished);

    m_scrollTimeLine=new QTimeLine(200, this);
    m_scrollTimeLine->setUpdateInterval(5);
    m_scrollTimeLine->setEasingCurve(QEasingCurve::OutCubic);
    connect(m_scrollTimeLine, SIGNAL(frameChanged(int)),
            verticalScrollBar(), SLOT(setValue(int)));

    connect(verticalScrollBar(), &QScrollBar::valueChanged,
            this, &KNMusicAlbumView::onActionScrolling);

    updateParameters();
}

QModelIndex KNMusicAlbumView::indexAt(const QPoint &point) const
{
    int pointTop=verticalScrollBar()->value()+point.y(),
        pointLine=pointTop/m_spacingHeight,
        pointColumn=point.x()/m_spacingWidth,
        horizontalPosition=point.x()-pointColumn*m_spacingWidth;
    if(pointTop-pointLine*m_spacingHeight<m_spacing ||
       horizontalPosition<m_spacing ||
       horizontalPosition>m_spacing+m_iconSizeParam)
    {
        //Clicked on space.
        return QModelIndex();
    }
    return m_proxyModel->index(pointLine*m_maxColumnCount+pointColumn,
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
    int atTopPosition=index.row()/m_maxColumnCount*m_spacingHeight;
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
        int bottomValue=atTopPosition-height()+m_spacingHeight,
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
        return QRect(rect.left()-horizontalScrollBar()->value(),
                     rect.top()-verticalScrollBar()->value(),
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

void KNMusicAlbumView::setCategoryModel(KNMusicCategorySortFilterModel *model)
{
    setModel(model);
    m_proxyModel=model;
    m_model=static_cast<KNMusicAlbumModel *>(model->sourceModel());
    connect(m_model, &KNMusicAlbumModel::albumRemoved,
            this, &KNMusicAlbumView::onActionAlbumRemoved);
    connect(m_model, &KNMusicAlbumModel::albumArtistChanged,
            this, &KNMusicAlbumView::onActionAlbumArtistChagned);
    connect(m_model, &KNMusicAlbumModel::requireFlyAway,
            this, &KNMusicAlbumView::flyAwayAlbumDetail);
}

void KNMusicAlbumView::setDetailModel(KNMusicAlbumDetailModel *model)
{
    m_detailModel=model;
    m_albumDetail->setDetailModel(m_detailModel);
}

void KNMusicAlbumView::selectCategoryItem(const QString &value)
{
    QModelIndex albumSearch;
    if(value.isEmpty())
    {
        albumSearch=m_model->index(0,0);
        expandAlbumDetail(albumSearch);
        //scrollTo(albumSearch, QAbstractItemView::PositionAtTop);
        return;
    }
    albumSearch=m_model->indexOf(value);
    if(albumSearch.isValid())
    {
        expandAlbumDetail(albumSearch);
        scrollTo(m_proxyModel->mapFromSource(albumSearch),
                 QAbstractItemView::PositionAtCenter);
    }
}

void KNMusicAlbumView::selectItem(const QModelIndex &index)
{
    QModelIndex testIndex=m_detailModel->mapFromSource(index);
    if(testIndex.isValid())
    {
        m_albumDetail->selectItem(testIndex);
    }
}

void KNMusicAlbumView::selectMusicItem(const QModelIndex &index)
{
    selectCategoryItem(m_musicModel->itemText(index.row(),
                                              KNMusicGlobal::Album));
    selectItem(index);
}

void KNMusicAlbumView::updateGeometries()
{
    int verticalMax=qMax(0,
                         m_lineCount*m_spacingHeight+m_spacing-height());
    verticalScrollBar()->setRange(0, verticalMax);
    verticalScrollBar()->setPageStep((m_iconSizeParam>>1)-m_spacing);
    verticalScrollBar()->setSingleStep((m_iconSizeParam>>1)-m_spacing);
}

void KNMusicAlbumView::paintEvent(QPaintEvent *event)
{
    QStyleOptionViewItem option=viewOptions();
    QBrush background=option.palette.base();
    QPen foreground(option.palette.color(QPalette::Text));
    QPainter painter(viewport());
    painter.setRenderHint(QPainter::TextAntialiasing);
    if(autoFillBackground())
    {
        painter.fillRect(event->rect(), background);
    }
    painter.setPen(foreground);

    updateParameters();

    int albumIndex=0, albumCount=m_proxyModel->rowCount(),
        currentRow=0, currentColumn=0,
        currentLeft=m_spacing, currentTop=m_spacing;
    m_lineCount=(albumCount+m_maxColumnCount-1)/m_maxColumnCount;

    painter.translate(0, -verticalScrollBar()->value());
    int skipLineCount=verticalScrollBar()->value()/(m_spacingHeight),
        drawnHeight=0, maxDrawnHeight=height()+m_spacingHeight;
    currentRow+=skipLineCount;
    currentTop+=m_spacingHeight*skipLineCount;
    albumIndex=skipLineCount*m_maxColumnCount;
    m_firstVisibleIndex=albumIndex;
    QModelIndex currentPaintIndex, sourceIndex;
    while(albumIndex < albumCount && drawnHeight < maxDrawnHeight)
    {
        currentPaintIndex=m_proxyModel->index(albumIndex, 0);
        sourceIndex=m_proxyModel->mapToSource(currentPaintIndex);
        QRect currentRect=QRect(currentLeft,
                                currentTop,
                                m_gridWidth,
                                m_gridHeight);
        if(sourceIndex!=m_detailIndex)
        {
            paintAlbum(&painter,
                       currentRect,
                       sourceIndex);
        }
        currentColumn++;
        if(currentColumn==m_maxColumnCount)
        {
            currentColumn=0;
            currentRow++;
            currentLeft=m_spacing;
            currentTop+=m_spacingHeight;
            drawnHeight+=m_spacingHeight;
        }
        else
        {
            currentLeft+=m_spacingWidth;
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
        int detailSizeParam=(height()>>2)*3,
            widthParam=(detailSizeParam>>1)*3;
        m_albumDetail->setGeometry((width()-widthParam)>>1,
                                   height()>>3,
                                   widthParam,
                                   detailSizeParam);
    }
}

void KNMusicAlbumView::keyReleaseEvent(QKeyEvent *event)
{
    switch(event->key())
    {
    case Qt::Key_Escape:
        foldAlbumDetail();
        break;
    }
    QAbstractItemView::keyReleaseEvent(event);
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
                                         Qt::KeyboardModifiers modifiers)
{
    QModelIndex current=currentIndex(), movedIndex;
    /*switch (cursorAction)
    {
    case QAbstractItemView::MoveUp:
        movedIndex=m_model->index(current.row()-m_maxColumnCount, 0);
        break;
    case QAbstractItemView::MoveDown:
        movedIndex=m_model->index(current.row()+m_maxColumnCount, 0);
        break;
    case QAbstractItemView::MoveLeft:
        movedIndex=m_model->index(current.row()-1, 0);
        break;
    case QAbstractItemView::MoveRight:
        movedIndex=m_model->index(current.row()+1, 0);
        break;
    case QAbstractItemView::MoveHome:
        movedIndex=m_model->index(0, 0);
        break;
    case QAbstractItemView::MoveEnd:
        movedIndex=m_model->index(m_model->rowCount()-1, 0);
        break;
    default:
        break;
    }*/
    //selectAlbum(movedIndex);
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
    m_pressedIndex=m_proxyModel->mapToSource(indexAt(e->pos()));
}

void KNMusicAlbumView::mouseReleaseEvent(QMouseEvent *e)
{
     QAbstractItemView::mouseReleaseEvent(e);
     if(m_pressedIndex==m_proxyModel->mapToSource(indexAt(e->pos())))
     {
         if(m_pressedIndex!=m_detailIndex)
         {
             selectAlbum(m_pressedIndex);
             viewport()->update();
             return;
         }
     }
     foldAlbumDetail();
     /*if(m_pressedIndex==indexAt(e->pos()) &&
        m_pressedIndex.isValid())
     {
         ;
     }*/
}

void KNMusicAlbumView::expandAlbumDetail(const QModelIndex &index)
{
    m_albumDetail->hide();
    m_albumDetail->disableArtworkExpand();
    if(!index.isValid())
    {
        return;
    }
    m_detailIndex=index;
    syncDetailData();
    m_detailModel->setCategoryIndex(m_detailIndex);
    m_albumDetail->resetSongState();
    QRect startPosition=visualRect(m_proxyModel->mapFromSource(m_detailIndex));
    m_albumDetail->setGeometry(startPosition.x()+2,
                               startPosition.y()+2,
                               m_iconSizeParam-2,
                               m_iconSizeParam-2);
    m_albumShow->setStartValue(m_albumDetail->geometry());
    m_albumShow->setEndValue(QRect((width()>>1)-(height()>>3)*3,
                                   (height()>>3),
                                   (height()>>2)*3,
                                   (height()>>2)*3));
    m_albumDetail->hideDetailWidget();
    m_albumDetail->show();
    m_hidingAlbum=false;
    m_albumShow->start();
}

void KNMusicAlbumView::onActionHideAlbumDetail()
{
    QRect endPosition=visualRect(m_proxyModel->mapFromSource(m_detailIndex));
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
    m_hidingAlbum=false;
}

void KNMusicAlbumView::onActionAlbumRemoved(const QModelIndex &index)
{
    if(index==m_detailIndex)
    {
        flyAwayAlbumDetail();
    }
}

void KNMusicAlbumView::onActionFlyAwayAlbumDetail()
{
    int param=m_iconSizeParam-2;
    QRect startPosition=m_albumDetail->geometry();
    m_albumThrow->setStartValue(startPosition);
    m_albumThrow->setEndValue(QRect(startPosition.x()+(startPosition.width()>>1),
                                    -param,
                                    0,
                                    0));
    m_flyawayGroup->start();
}

void KNMusicAlbumView::onActionFlyAwayAlbumDetailFinished()
{
    m_albumDetail->hide();
    viewport()->update();
    m_flyingAlbum=false;
}

void KNMusicAlbumView::onActionScrolling()
{
    if(m_albumHide->state()==QAbstractAnimation::Running)
    {
        QRect endPosition=visualRect(m_proxyModel->mapFromSource(m_detailIndex));
        m_albumHide->setEndValue(QRect(endPosition.x()+2,
                                       endPosition.y()+2,
                                       m_iconSizeParam-2,
                                       m_iconSizeParam-2));
    }
}

void KNMusicAlbumView::onActionAlbumArtistChagned(const QModelIndex &index)
{
    update();
    if(m_detailIndex==index)
    {
        m_model->indexArtist(index);
        syncDetailData();
    }
}

void KNMusicAlbumView::syncDetailData()
{
    QIcon currentIcon=m_model->data(m_detailIndex, Qt::DecorationRole).value<QIcon>();
    m_albumDetail->setAlbumArt(currentIcon.pixmap(height(), height()),
                               QSize(height(), height()));
    m_albumDetail->setAlbumText(m_model->data(m_detailIndex).toString());
    m_albumDetail->setArtistText(m_model->indexArtist(m_detailIndex));
    m_albumDetail->setYearText(m_model->indexYear(m_detailIndex));
}

void KNMusicAlbumView::flyAwayAlbumDetail()
{
    if(m_albumDetail->isVisible() && !m_flyingAlbum)
    {
        m_flyingAlbum=true;
        selectionModel()->clear();
        m_detailIndex=QModelIndex();
        m_albumDetail->flyAway();
        viewport()->update();
    }
}

void KNMusicAlbumView::foldAlbumDetail()
{
    if(m_albumDetail->isVisible() && !m_hidingAlbum)
    {
        m_hidingAlbum=true;
        m_albumDetail->foldDetail();
        selectionModel()->clear();
    }
}

QRect KNMusicAlbumView::itemRect(const QModelIndex &index) const
{
    if(!index.isValid())
    {
        return QRect();
    }
    int itemIndex=index.row(),
        itemLine=itemIndex/m_maxColumnCount,
        itemColumn=itemIndex-itemLine*m_maxColumnCount;
    return QRect(itemColumn*m_spacingWidth+m_spacing,
                 itemLine*m_spacingHeight+m_spacing,
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
    m_spacingHeight=m_gridHeight+m_spacing;
    m_spacingWidth=m_gridWidth+m_spacing;
}

void KNMusicAlbumView::paintAlbum(QPainter *painter,
                                  const QRect &rect,
                                  const QModelIndex &index)
{
    if(!index.isValid())
    {
        return;
    }
    //To draw the album art.
    QModelIndex originalIndex=index;
    QIcon currentIcon=m_model->data(originalIndex, Qt::DecorationRole).value<QIcon>();
    QRect albumArtRect=QRect(rect.x()+1,
                             rect.y()+1,
                             m_iconSizeParam-2,
                             m_iconSizeParam-2);
    painter->drawPixmap(albumArtRect,
                        currentIcon.pixmap(m_iconSizeParam, m_iconSizeParam));

    //To draw the text.
    int textTop=rect.y()+m_iconSizeParam+5,
        textWidth=rect.width()-m_spacing;
    painter->drawText(rect.x(),
                      textTop,
                      textWidth,
                      fontMetrics().height(),
                      Qt::TextSingleLine | Qt::AlignLeft | Qt::AlignTop,
                      fontMetrics().elidedText(m_model->data(originalIndex).toString(),
                                               Qt::ElideRight,
                                               textWidth));
    textTop+=fontMetrics().height();
    QColor penBackup=painter->pen().color();
    painter->setPen(QColor(128,128,128));
    painter->drawText(rect.x(),
                      textTop,
                      textWidth,
                      fontMetrics().height(),
                      Qt::TextSingleLine | Qt::AlignLeft | Qt::AlignTop,
                      fontMetrics().elidedText(m_model->indexArtist(originalIndex),
                                               Qt::ElideRight,
                                               textWidth));
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

void KNMusicAlbumView::setMusicSourceModel(KNMusicLibraryModelBase *model)
{
    m_musicModel=model;
    m_albumDetail->setSourceModel(m_musicModel);
    m_detailModel->setSourceModel(m_musicModel);
}

void KNMusicAlbumView::setMusicBackend(KNMusicBackend *backend)
{
    m_albumDetail->setMusicBackend(backend);
}

void KNMusicAlbumView::setFilterFixedString(const QString &text)
{
    flyAwayAlbumDetail();
    m_proxyModel->setFilterFixedString(text);
    viewport()->update();
}

void KNMusicAlbumView::selectAlbum(const QModelIndex &index)
{
    if(index.isValid())
    {
        expandAlbumDetail(index);
    }
    else
    {
        foldAlbumDetail();
    }
}
