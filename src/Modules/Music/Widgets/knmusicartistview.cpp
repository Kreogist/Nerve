#include <QItemSelectionModel>
#include <QSplitter>
#include <QLabel>
#include <QHeaderView>
#include <QBoxLayout>

#include <QDebug>

#include "../Libraries/knmusicartistitem.h"
#include "../Libraries/knmusiccategorymodel.h"
#include "../Libraries/knmusiccategorydetailmodel.h"

#include "../knmusicglobal.h"

#include "knmusicartistlist.h"

#include "knmusicartistview.h"

KNMusicArtistSongs::KNMusicArtistSongs(QWidget *parent) :
    KNMusicListView(parent)
{
    ;
}

void KNMusicArtistSongs::resetHeader()
{
    for(int i=KNMusicGlobal::Name+1;
        i<KNMusicGlobal::MusicDataCount;
        i++)
    {
        setColumnHidden(i, true);
    }
    setColumnHidden(KNMusicGlobal::Time, false);
    setColumnHidden(KNMusicGlobal::Album, false);
    setColumnHidden(KNMusicGlobal::Genre, false);
    setColumnHidden(KNMusicGlobal::Rating, false);
    moveToFirst(KNMusicGlobal::Rating);
    moveToFirst(KNMusicGlobal::Genre);
    moveToFirst(KNMusicGlobal::Album);
    moveToFirst(KNMusicGlobal::Time);
    moveToFirst(KNMusicGlobal::Name);
}

KNMusicCategoryDetailsDisplay::KNMusicCategoryDetailsDisplay(QWidget *parent) :
    QWidget(parent)
{
    setAutoFillBackground(true);
    setContentsMargins(0,0,0,0);
    QBoxLayout *m_layout=new QBoxLayout(QBoxLayout::TopToBottom, this);
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

    m_songViewer=new KNMusicArtistSongs(this);
    connect(m_songViewer, &KNMusicArtistSongs::requireOpenUrl,
            this, &KNMusicCategoryDetailsDisplay::requireOpenUrl);
    connect(m_songViewer, &KNMusicArtistSongs::requireShowContextMenu,
            this, &KNMusicCategoryDetailsDisplay::requireShowContextMenu);
    m_layout->addWidget(m_songViewer, 1);
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
}

void KNMusicCategoryDetailsDisplay::resetHeader()
{
    m_songViewer->resetHeader();
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

    m_artistList=new KNMusicArtistList(this);
    addWidget(m_artistList);

    m_artistDetails=new KNMusicCategoryDetailsDisplay(this);
    connect(m_artistDetails, &KNMusicCategoryDetailsDisplay::requireOpenUrl,
            this, &KNMusicArtistView::requireOpenUrl);
    connect(m_artistDetails, &KNMusicCategoryDetailsDisplay::requireShowContextMenu,
            this, &KNMusicArtistView::requireShowContextMenu);
    addWidget(m_artistDetails);
    setCollapsible(1, false);
    setStretchFactor(1, 1);
}

void KNMusicArtistView::resetHeader()
{
    m_artistDetails->resetHeader();
}

void KNMusicArtistView::setModel(KNMusicCategoryModel *model)
{
    m_artistList->setModel(model);
    connect(m_artistList->selectionModel(), &QItemSelectionModel::currentChanged,
            this, &KNMusicArtistView::onActionItemActivate);
    m_artistModel=model;
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
    QList<QStandardItem *> artistSearch=m_artistModel->findItems(value);
    if(artistSearch.size()==0)
    {
        return;
    }
    m_artistList->selectionModel()->setCurrentIndex(artistSearch.at(0)->index(),
                                                    QItemSelectionModel::SelectCurrent);
}

void KNMusicArtistView::selectItem(const QModelIndex &index)
{
    QModelIndex testIndex=m_artistDetailModel->mapFromSource(index);
    if(testIndex.isValid())
    {
        m_artistDetails->setCurrentIndex(testIndex);
    }
}

void KNMusicArtistView::resort()
{
    m_artistList->model()->sort(0);
}

void KNMusicArtistView::onActionSongCountChange(const int &value)
{
    m_artistDetails->setSongNumber(value);
}

void KNMusicArtistView::onActionItemActivate(const QModelIndex &current,
                                             const QModelIndex &previous)
{
    Q_UNUSED(previous);
    m_artistDetails->setArtistName(m_artistModel->data(current,Qt::DisplayRole).toString());
    m_artistDetailModel->setCategoryIndex(current);
}
