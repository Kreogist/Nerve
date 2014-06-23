#include <QTimeLine>
#include <QPainter>

#include <QDebug>

#include "knmusiclistviewheadermenu.h"

#include "knmusiclistviewheader.h"

KNMusicListViewHeader::KNMusicListViewHeader(QWidget *parent) :
    QHeaderView(Qt::Horizontal, parent)
{
    //Set properties.
    setAutoFillBackground(true);
    setAutoScroll(true);
    setContentsMargins(0,0,0,0);
    setContextMenuPolicy(Qt::CustomContextMenu);
    setDefaultAlignment(Qt::AlignLeft);
    setFrameShape(QFrame::NoFrame);
    setFrameShadow(QFrame::Plain);
    setSectionsMovable(true);
    //setStretchLastSection(true);

    m_viewerMenu=new KNMusicListViewHeaderMenu(this);
    connect(this, &KNMusicListViewHeader::customContextMenuRequested,
            this, &KNMusicListViewHeader::showContextMenu);
    connect(m_viewerMenu, &KNMusicListViewHeaderMenu::requireChangeVisible,
            this, &KNMusicListViewHeader::requireChangeVisible);

    //Set palette.
    m_backgroundColor=m_lineColor;
    m_palette=palette();
    m_palette.setColor(QPalette::Base,          QColor(0,0,0,0));
    m_palette.setColor(QPalette::Window,        QColor(0,0,0,0));
    buttonGradient=QLinearGradient(QPoint(0,0), QPoint(0, height()));
    buttonGradient.setColorAt(0, QColor(0x48,0x48,0x48));
    buttonGradient.setColorAt(1, QColor(0x48,0x48,0x48));
    m_palette.setBrush(QPalette::Button,        QBrush(buttonGradient));
    m_palette.setColor(QPalette::ButtonText,    QColor(0xbf, 0xbf, 0xbf));
    setPalette(m_palette);

    m_mouseIn=new QTimeLine(200, this);
    m_mouseIn->setEasingCurve(QEasingCurve::OutCubic);
    m_mouseIn->setUpdateInterval(5);
    m_mouseIn->setEndFrame(0x40);
    connect(m_mouseIn, &QTimeLine::frameChanged,
            this, &KNMusicListViewHeader::changeBackground);

    m_mouseOut=new QTimeLine(200, this);
    m_mouseOut->setEasingCurve(QEasingCurve::OutCubic);
    m_mouseOut->setUpdateInterval(5);
    m_mouseOut->setEndFrame(0x20);
    connect(m_mouseOut, &QTimeLine::frameChanged,
            this, &KNMusicListViewHeader::changeBackground);
}

void KNMusicListViewHeader::hideStrectch()
{
    //setSectionHidden();
}

void KNMusicListViewHeader::showStrectch()
{
    ;
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

void KNMusicListViewHeader::paintSection(QPainter *painter,
                                         const QRect &rect,
                                         int logicalIndex) const
{
    QRect contentRect;
    if(logicalIndex==sortIndicatorSection())
    {
        painter->drawPixmap(QPoint(rect.x()+rect.width()-20,
                                   rect.y()+((rect.height()-20)>>1)),
                            sortIndicatorOrder()==Qt::AscendingOrder?
                                QPixmap(":/Music/Resources/Public/AscendingIndicator.png"):
                                QPixmap(":/Music/Resources/Public/DescendingIndicator.png"));
        contentRect=QRect(rect.x()+4,
                          rect.y()+1,
                          rect.width()-28,
                          rect.height()-2);
    }
    else
    {
        contentRect=QRect(rect.x()+4,
                          rect.y()+1,
                          rect.width()-8,
                          rect.height()-2);
    }
    painter->drawText(contentRect,
                      model()->headerData(logicalIndex, Qt::Horizontal, Qt::TextAlignmentRole).toInt(),
                      model()->headerData(logicalIndex, Qt::Horizontal).toString());
    painter->setPen(m_lineColor);
    painter->drawLine(rect.topRight(), rect.bottomRight());
}

void KNMusicListViewHeader::changeBackground(int frameData)
{
    m_backgroundColor=QColor(frameData, frameData, frameData);
    int textParam=(frameData<<1);
    m_lineColor=QColor(textParam, textParam, textParam);
    buttonGradient.setColorAt(1, QColor(frameData+0x38,frameData+0x38,frameData+0x38));
    m_palette.setBrush(QPalette::Button, QBrush(buttonGradient));
    textParam+=127;
    m_palette.setColor(QPalette::ButtonText, QColor(textParam,
                                                    textParam,
                                                    textParam));
    setPalette(m_palette);
}

void KNMusicListViewHeader::showContextMenu(const QPoint &mousePoint)
{
    for(int i=0; i<KNMusicGlobal::MusicDataCount; i++)
    {
        m_viewerMenu->setHeaderValue(i, !isSectionHidden(i));
    }
    m_viewerMenu->exec(mapToGlobal(mousePoint));
}
