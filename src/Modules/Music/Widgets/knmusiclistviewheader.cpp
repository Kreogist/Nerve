#include <QTimeLine>

#include "knmusiclistviewheadermenu.h"

#include "knmusiclistviewheader.h"

KNMusicListViewHeader::KNMusicListViewHeader(QWidget *parent) :
    QHeaderView(Qt::Horizontal, parent)
{
    //Set properties.
    setSectionsMovable(true);
    setDefaultAlignment(Qt::AlignLeft);
    setContextMenuPolicy(Qt::CustomContextMenu);

    m_viewerMenu=new KNMusicListViewHeaderMenu(this);
    connect(this, &KNMusicListViewHeader::customContextMenuRequested,
            this, &KNMusicListViewHeader::showContextMenu);
    connect(m_viewerMenu, &KNMusicListViewHeaderMenu::requireChangeVisible,
            this, &KNMusicListViewHeader::requireChangeVisible);

    //Set palette.
    int minGrey=0x20;
    m_backgroundColor=QColor(minGrey, minGrey, minGrey);
    m_palette=palette();
    m_palette.setColor(QPalette::Base, m_backgroundColor);
    m_palette.setColor(QPalette::Window, m_backgroundColor);
    m_palette.setColor(QPalette::Button, m_backgroundColor);
    m_palette.setColor(QPalette::ButtonText, QColor(0xff, 0xff, 0xff));
    setPalette(m_palette);

    m_mouseIn=new QTimeLine(200, this);
    m_mouseIn->setEasingCurve(QEasingCurve::OutCubic);
    m_mouseIn->setUpdateInterval(2);
    m_mouseIn->setEndFrame(0x40);
    connect(m_mouseIn, &QTimeLine::frameChanged,
            this, &KNMusicListViewHeader::changeBackground);

    m_mouseOut=new QTimeLine(200, this);
    m_mouseOut->setEasingCurve(QEasingCurve::OutCubic);
    m_mouseOut->setUpdateInterval(2);
    m_mouseOut->setEndFrame(minGrey);
    connect(m_mouseOut, &QTimeLine::frameChanged,
            this, &KNMusicListViewHeader::changeBackground);
}

void KNMusicListViewHeader::moveToFirst(int logicalIndex)
{
    moveSection(visualIndex(logicalIndex), 0);
}

void KNMusicListViewHeader::enterEvent(QEvent *e)
{
    m_mouseOut->stop();
    m_mouseIn->stop();
    m_mouseIn->setStartFrame(m_backgroundColor.red());
    m_mouseIn->start();
    QHeaderView::enterEvent(e);
}

void KNMusicListViewHeader::leaveEvent(QEvent *e)
{
    m_mouseIn->stop();
    m_mouseOut->stop();
    m_mouseOut->setStartFrame(m_backgroundColor.red());
    m_mouseOut->start();
    QHeaderView::leaveEvent(e);
}

void KNMusicListViewHeader::changeBackground(int frameData)
{
    m_backgroundColor=QColor(frameData, frameData, frameData);
    m_palette.setColor(QPalette::Base, m_backgroundColor);
    m_palette.setColor(QPalette::Window, m_backgroundColor);
    m_palette.setColor(QPalette::Button, m_backgroundColor);
    setPalette(m_palette);
}

void KNMusicListViewHeader::showContextMenu(const QPoint &mousePoint)
{
    for(int i=0; i<KNMusicGlobal::MusicDataCount; i++)
    {
        m_viewerMenu->setHeaderValue(i, !isSectionHidden(i));
    }
    m_viewerMenu->exec(mousePoint);
}
