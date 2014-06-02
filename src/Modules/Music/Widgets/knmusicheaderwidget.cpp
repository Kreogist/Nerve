#include <QBoxLayout>
#include <QResizeEvent>

#include "../../Base/knsearchbox.h"
#include "../Libraries/knmusicmodel.h"
#include "../knmusicglobal.h"
#include "knmusicheaderplayer.h"

#include "knmusicheaderwidget.h"

KNMusicHeaderWidget::KNMusicHeaderWidget(QWidget *parent) :
    KNStdLibHeaderWidget(parent)
{
    setContentsMargins(0,0,0,0);
    m_mainLayout=new QBoxLayout(QBoxLayout::LeftToRight, this);
    m_mainLayout->setAlignment(Qt::AlignVCenter);
    setLayout(m_mainLayout);

    m_headerPlayer=new KNMusicHeaderPlayer(this);
//    m_visualEffect=m_headerPlayer->visualEffect();
//    m_visualEffect->setParent(this);
    connect(m_headerPlayer, &KNMusicHeaderPlayer::requireShowMusicPlayer,
            this, &KNMusicHeaderWidget::requireShowMusicPlayer);
    connect(m_headerPlayer, &KNMusicHeaderPlayer::requireHideMusicPlayer,
            this, &KNMusicHeaderWidget::requireHideMusicPlayer);
    m_mainLayout->addWidget(m_headerPlayer, 1, Qt::AlignLeft | Qt::AlignVCenter);

    m_searchBox=new KNSearchBox(this);
    m_searchBox->setFixedWidth(200);
    m_searchBox->setPlaceHolderText(m_searchPlaceHolder);
    connect(m_searchBox, &KNSearchBox::textEdited,
            this, &KNMusicHeaderWidget::requireSearch);
    connect(m_searchBox, &KNSearchBox::requireLostFocus,
            this, &KNMusicHeaderWidget::requireLostFocus);
    m_mainLayout->addWidget(m_searchBox, 0, Qt::AlignRight | Qt::AlignVCenter);

//    m_visualEffect->lower();
}

void KNMusicHeaderWidget::setMusicModel(KNMusicModel *model)
{
    m_musicModel=model;
}

void KNMusicHeaderWidget::setMusicPlayer(KNMusicPlayer *player)
{
    m_headerPlayer->setPlayer(player);
}

void KNMusicHeaderWidget::retranslate()
{
    m_searchPlaceHolder=tr("Search Music");
}

void KNMusicHeaderWidget::retranslateAndSet()
{
    retranslate();
    m_searchBox->setPlaceHolderText(m_searchPlaceHolder);
}

void KNMusicHeaderWidget::setSearchFocus()
{
    m_searchBox->setSearchFocus();
}

void KNMusicHeaderWidget::clearSearch()
{
    m_searchBox->clear();
}

void KNMusicHeaderWidget::onActionPlayMusic(const QModelIndex &index)
{
    m_currentIndex=index;
    m_headerPlayer->setAlbumArt(QPixmap::fromImage(m_musicModel->artwork(m_currentIndex.row())));
    m_headerPlayer->setTitle(m_musicModel->itemText(m_currentIndex.row(), KNMusicGlobal::Name));
    m_headerPlayer->setArtist(m_musicModel->itemText(m_currentIndex.row(), KNMusicGlobal::Artist));
    m_headerPlayer->playFile(m_musicModel->filePathFromIndex(m_currentIndex.row()));
}

void KNMusicHeaderWidget::resizeEvent(QResizeEvent *event)
{
    KNStdLibHeaderWidget::resizeEvent(event);
//    m_visualEffect->move(m_searchBox->x()-m_visualEffect->width(),
//                         0);
}
