#include <QItemSelectionModel>
#include <QSplitter>
#include <QScrollBar>
#include <QLabel>
#include <QTimeLine>
#include <QBoxLayout>

#include <QDebug>

#include "../Libraries/knmusicartistitem.h"
#include "../Libraries/knmusicartistmodel.h"
#include "../Libraries/knmusicartistdetailmodel.h"

#include "knmusicartistview.h"

KNMusicArtistSongs::KNMusicArtistSongs(QWidget *parent) :
    QTreeView(parent)
{
    ;
}

KNMusicArtistDetailsDisplay::KNMusicArtistDetailsDisplay(QWidget *parent) :
    QWidget(parent)
{
    setContentsMargins(0,0,0,0);
    QBoxLayout *m_layout=new QBoxLayout(QBoxLayout::TopToBottom, this);
    m_layout->setContentsMargins(0,0,0,0);
    m_layout->setSpacing(0);
    setLayout(m_layout);

    m_song=tr("1 song");
    m_songs=tr("%1 songs");

    m_artistName=new QLabel(this);
    m_layout->addWidget(m_artistName);
    m_artistInfo=new QLabel(this);
    m_layout->addWidget(m_artistInfo);
    m_songViewer=new KNMusicArtistSongs(this);
    m_layout->addWidget(m_songViewer, 1);
}

void KNMusicArtistDetailsDisplay::setArtistName(const QString &artistName)
{
    m_artistName->setText(artistName);
}

void KNMusicArtistDetailsDisplay::setSongNumber(const int &index)
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

void KNMusicArtistDetailsDisplay::setDetailModel(KNMusicArtistDetailModel *model)
{
    m_songViewer->setModel(model);
}

KNMusicArtistList::KNMusicArtistList(QWidget *parent) :
    QListView(parent)
{
    setIconSize(QSize(40, 40));
    setSelectionMode(QAbstractItemView::SingleSelection);
    setUniformItemSizes(true);
    setSpacing(2);
    setVerticalScrollMode(QAbstractItemView::ScrollPerPixel);
    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    m_backgroundColor=QColor(m_minGrey, m_minGrey, m_minGrey);
    m_palette=palette();
    m_palette.setColor(QPalette::Base, m_backgroundColor);
    m_palette.setColor(QPalette::Window, QColor(0x20, 0x20, 0x20));
    m_palette.setColor(QPalette::Button, QColor(0x20, 0x20, 0x20));
    m_palette.setColor(QPalette::Text, QColor(0xff, 0xff, 0xff));
    m_palette.setColor(QPalette::Highlight, QColor(0x60, 0x60, 0x60));
    m_palette.setColor(QPalette::HighlightedText, QColor(0xf7, 0xcf, 0x3d));
    setPalette(m_palette);

    m_mouseIn=new QTimeLine(200, this);
    m_mouseIn->setUpdateInterval(5);
    m_mouseIn->setEndFrame(0x40);
    connect(m_mouseIn, &QTimeLine::frameChanged,
            this, &KNMusicArtistList::changeBackground);

    m_mouseOut=new QTimeLine(200, this);
    m_mouseOut->setUpdateInterval(5);
    m_mouseOut->setEndFrame(m_minGrey);
    connect(m_mouseOut, &QTimeLine::frameChanged,
            this, &KNMusicArtistList::changeBackground);
}

void KNMusicArtistList::enterEvent(QEvent *e)
{
    m_mouseOut->stop();
    m_mouseIn->stop();
    m_mouseIn->setStartFrame(m_backgroundColor.red());
    m_mouseIn->start();
    QListView::enterEvent(e);
}

void KNMusicArtistList::leaveEvent(QEvent *e)
{
    m_mouseIn->stop();
    m_mouseOut->stop();
    m_mouseOut->setStartFrame(m_backgroundColor.red());
    m_mouseOut->start();
    QListView::leaveEvent(e);
}

void KNMusicArtistList::changeBackground(int frameData)
{
    m_backgroundColor=QColor(frameData, frameData, frameData);
    int baseGrey=((frameData-m_minGrey)>>1)+m_minGrey;
    m_palette.setColor(QPalette::Base, QColor(baseGrey, baseGrey, baseGrey));
    m_palette.setColor(QPalette::Button, m_backgroundColor);
    setPalette(m_palette);
}

KNMusicArtistView::KNMusicArtistView(QWidget *parent) :
    QSplitter(parent)
{
    setContentsMargins(0,0,0,0);
    setHandleWidth(2);

    m_artistList=new KNMusicArtistList(this);
    addWidget(m_artistList);

    m_artistDetails=new KNMusicArtistDetailsDisplay(this);
    addWidget(m_artistDetails);
    setCollapsible(1, false);
    setStretchFactor(1, 1);
}

void KNMusicArtistView::setModel(KNMusicArtistModel *model)
{
    m_artistList->setModel(model);
    connect(m_artistList->selectionModel(), &QItemSelectionModel::currentChanged,
            this, &KNMusicArtistView::onActionItemActivate);
    m_artistModel=model;
}

void KNMusicArtistView::setDetailModel(KNMusicArtistDetailModel *model)
{
    m_artistDetails->setDetailModel(model);
    connect(this, SIGNAL(requireDisplayDetails(QModelIndex)),
            model, SLOT(setArtistIndex(QModelIndex)));
}

void KNMusicArtistView::resort()
{
    m_artistList->model()->sort(0);
}

void KNMusicArtistView::onActionItemActivate(const QModelIndex &current,
                                             const QModelIndex &previous)
{
    Q_UNUSED(previous);
    m_artistDetails->setArtistName(m_artistModel->data(current,Qt::DisplayRole).toString());
    KNMusicArtistItem *currentArtist=
            static_cast<KNMusicArtistItem *>(m_artistModel->itemFromIndex(current));
    m_artistDetails->setSongNumber(currentArtist->songs().count());
    emit requireDisplayDetails(current);
}
