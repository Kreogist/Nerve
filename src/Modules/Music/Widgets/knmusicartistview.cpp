#include <QItemSelectionModel>
#include <QSplitter>
#include <QLabel>
#include <QShowEvent>
#include <QBitmap>
#include <QGraphicsOpacityEffect>
#include <QHeaderView>
#include <QBoxLayout>
#include <QResizeEvent>

#include <QDebug>

#include "../Libraries/knmusicartistitem.h"
#include "../Libraries/knmusiccategorymodel.h"
#include "../Libraries/knmusiccategorydetailmodel.h"
#include "../Libraries/knmusiccategorysortfiltermodel.h"

#include "../knmusicglobal.h"

#include "knmusiccategorylist.h"
#include "knmusiclistviewbase.h"

#include "knmusicartistview.h"

KNMusicCategoryDetailsDisplay::KNMusicCategoryDetailsDisplay(QWidget *parent) :
    QWidget(parent)
{
    setAutoFillBackground(true);
    setContentsMargins(0,0,0,0);

    m_largeIcon=new QLabel(this);
    m_largeIcon->installEventFilter(this);
    m_largeIcon->setScaledContents(true);
    m_largeIcon->setGeometry(width()-256,
                             height()-256,
                             256,
                             256);
    m_opacityEffect=new QGraphicsOpacityEffect(m_largeIcon);
    m_opacityEffect->setOpacity(0.7);
    m_alphaGradient=QRadialGradient(QPointF(256,256),
                                    256,
                                    QPointF(256,256));
    m_alphaGradient.setColorAt(0.0, Qt::black);
    m_alphaGradient.setColorAt(1.0, Qt::transparent);
    m_opacityEffect->setOpacityMask(m_alphaGradient);
    m_largeIcon->setGraphicsEffect(m_opacityEffect);

    m_layout=new QBoxLayout(QBoxLayout::TopToBottom, this);
    m_layout->setContentsMargins(0,0,0,0);
    m_layout->setSpacing(0);
    setLayout(m_layout);

    QPalette pal=palette();
    pal.setColor(QPalette::Base, QColor(0x20, 0x20, 0x20));
    pal.setColor(QPalette::Window, QColor(0x20, 0x20, 0x20));
    pal.setColor(QPalette::Button, QColor(0x20, 0x20, 0x20));
    pal.setColor(QPalette::Text, QColor(0xff, 0xff, 0xff));
    pal.setColor(QPalette::Highlight, QColor(0x60, 0x60, 0x60));
    pal.setColor(QPalette::HighlightedText, QColor(0xf7, 0xcf, 0x3d));
    setPalette(pal);

    m_song=tr("1 song");
    m_songs=tr("%1 songs");

    m_layout->addSpacing(20);
    m_artistName=new QLabel(this);
    m_artistName->setContentsMargins(20,0,0,0);
    QFont artistFont=font();
    artistFont.setPointSize(artistFont.pointSize()+(artistFont.pointSize()>>1));
    artistFont.setBold(true);
    m_artistName->setFont(artistFont);
    pal=m_artistName->palette();
    pal.setColor(QPalette::WindowText, QColor(0xff, 0xff, 0xff));
    m_artistName->setPalette(pal);
    m_layout->addWidget(m_artistName);

    m_artistInfo=new QLabel(this);
    m_artistInfo->setContentsMargins(20,0,0,0);
    m_artistInfo->setPalette(pal);
    m_layout->addWidget(m_artistInfo);

    m_layout->addSpacing(20);
}

void KNMusicCategoryDetailsDisplay::setArtistName(const QString &artistName)
{
    m_artistName->setText(artistName);
}

void KNMusicCategoryDetailsDisplay::setSongNumber(const int &index)
{
    if(index==1)
    {
        m_artistInfo->setText(m_song);
    }
    else
    {
        m_artistInfo->setText(m_songs.arg(QString::number(index)));
    }
}

void KNMusicCategoryDetailsDisplay::setDetailModel(KNMusicCategoryDetailModel *model)
{
    m_songViewer->setModel(model);
    m_songViewer->resetHeader();
}

void KNMusicCategoryDetailsDisplay::setCurrentIndex(const QModelIndex &index)
{
    m_songViewer->setCurrentIndex(index);
    m_songViewer->scrollTo(index, QAbstractItemView::PositionAtCenter);
}

void KNMusicCategoryDetailsDisplay::setSongListView(KNMusicListViewBase *listview)
{
    m_songViewer=listview;
    m_layout->addWidget(m_songViewer, 1);
}

void KNMusicCategoryDetailsDisplay::setBackground(const QIcon &background)
{
    m_largeIcon->setPixmap(background.pixmap(m_largeIcon->size()));
}

void KNMusicCategoryDetailsDisplay::resetHeader()
{
    m_songViewer->resetHeader();
}

void KNMusicCategoryDetailsDisplay::resizeEvent(QResizeEvent *event)
{
    QWidget::resizeEvent(event);
    int songViewerHeight=m_songViewer->height();
    m_largeIcon->setGeometry(event->size().width()-songViewerHeight,
                             event->size().height()-songViewerHeight,
                             songViewerHeight,
                             songViewerHeight);
    m_alphaGradient.setCenter(QPointF(songViewerHeight, songViewerHeight));
    m_alphaGradient.setFocalPoint(QPointF(songViewerHeight, songViewerHeight));
    m_alphaGradient.setRadius(songViewerHeight);
    m_opacityEffect->setOpacityMask(m_alphaGradient);
}

KNMusicArtistView::KNMusicArtistView(QWidget *parent) :
    QSplitter(parent)
{
    setContentsMargins(0,0,0,0);
    setChildrenCollapsible(false);
    setOpaqueResize(false);
    setHandleWidth(0);

    QPalette pal=palette();
    pal.setColor(QPalette::Base, QColor(0x20, 0x20, 0x20));
    pal.setColor(QPalette::Window, QColor(0x20, 0x20, 0x20));
    pal.setColor(QPalette::Button, QColor(0x20, 0x20, 0x20));
    pal.setColor(QPalette::Text, QColor(0xff, 0xff, 0xff));
    pal.setColor(QPalette::Highlight, QColor(0x60, 0x60, 0x60));
    pal.setColor(QPalette::HighlightedText, QColor(0xf7, 0xcf, 0x3d));
    setPalette(pal);

    m_artistList=new KNMusicCategoryList(this);
    m_artistList->installEventFilter(this);
    addWidget(m_artistList);

    m_artistDetails=new KNMusicCategoryDetailsDisplay(this);
    m_artistDetails->installEventFilter(this);
    addWidget(m_artistDetails);
    setCollapsible(1, false);
    setStretchFactor(1, 1);
}

void KNMusicArtistView::resetHeader()
{
    m_artistDetails->resetHeader();
}

void KNMusicArtistView::setModel(KNMusicCategorySortFilterModel *model)
{
    m_artistList->setModel(model);
    m_proxyModel=model;
    connect(m_artistList->selectionModel(), &QItemSelectionModel::currentChanged,
            this, &KNMusicArtistView::onActionItemActivate);
    m_artistModel=static_cast<KNMusicCategoryModel *>(m_proxyModel->sourceModel());
}

void KNMusicArtistView::setDetailModel(KNMusicCategoryDetailModel *model)
{
    m_artistDetails->setDetailModel(model);
    connect(model, &KNMusicCategoryDetailModel::requireSongCountChange,
            this, &KNMusicArtistView::onActionSongCountChange);
    m_artistDetailModel=model;
}

void KNMusicArtistView::selectCategoryItem(const QString &value)
{
    QModelIndex artistSearch=m_proxyModel->mapFromSource(m_artistModel->indexOf(value));
    if(artistSearch.isValid())
    {
        m_artistList->selectionModel()->setCurrentIndex(artistSearch,
                                                        QItemSelectionModel::SelectCurrent);
    }
}

void KNMusicArtistView::selectItem(const QModelIndex &index)
{
    QModelIndex testIndex=m_artistDetailModel->mapFromSource(index);
    if(testIndex.isValid())
    {
        m_artistDetails->setCurrentIndex(testIndex);
    }
}

void KNMusicArtistView::setSongListView(KNMusicListViewBase *listview)
{
    m_artistDetails->setSongListView(listview);
}

void KNMusicArtistView::onActionSongCountChange(const int &value)
{
    m_artistDetails->setSongNumber(value);
}

void KNMusicArtistView::showEvent(QShowEvent *event)
{
    QSplitter::showEvent(event);
    if(!m_artistList->selectionModel()->currentIndex().isValid())
    {
        if(m_proxyModel->rowCount()!=0)
        {
            m_artistList->selectionModel()->setCurrentIndex(m_proxyModel->firstIndex(),
                                                            QItemSelectionModel::SelectCurrent);
        }
    }
}

void KNMusicArtistView::onActionItemActivate(const QModelIndex &current,
                                             const QModelIndex &previous)
{
    Q_UNUSED(previous);
    if(current.isValid())
    {
        QModelIndex originalIndex=m_proxyModel->mapToSource(current);
        m_artistDetails->setArtistName(m_proxyModel->data(current,Qt::DisplayRole).toString());
        m_artistDetails->setBackground(m_artistModel->albumArt(originalIndex));
        m_artistDetailModel->setCategoryIndex(originalIndex);
    }
}
