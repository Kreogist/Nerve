#include <QTimeLine>

#include "../knmusicglobal.h"

#include <QDebug>

#include "knmusiclistview.h"

KNMusicListView::KNMusicListView(QWidget *parent) :
    KNMusicLibraryListview(parent)
{
    setObjectName("KNMusicListView");
    //Set palette.
    int minGrey=0x10;
    m_backgroundColor=QColor(minGrey, minGrey, minGrey);
    m_palette=palette();
    m_palette.setColor(QPalette::Base, m_backgroundColor);
    m_palette.setColor(QPalette::AlternateBase, QColor(0x20, 0x20, 0x20));
    m_palette.setColor(QPalette::Window, QColor(0x20, 0x20, 0x20));
    m_palette.setColor(QPalette::Button, QColor(0x20, 0x20, 0x20));
    m_palette.setColor(QPalette::Text, QColor(0x9f, 0x9f, 0x9f, 0xCF));
    m_palette.setColor(QPalette::Highlight, QColor(0x60, 0x60, 0x60));
    m_palette.setColor(QPalette::HighlightedText, QColor(0xf7, 0xcf, 0x3d));
    setPalette(m_palette);

    m_mouseIn=new QTimeLine(200, this);
    m_mouseIn->setUpdateInterval(5);
    m_mouseIn->setEasingCurve(QEasingCurve::OutCubic);
    m_mouseIn->setEndFrame(0x30);
    connect(m_mouseIn, &QTimeLine::frameChanged,
            this, &KNMusicListView::changeBackground);

    m_mouseOut=new QTimeLine(200, this);
    m_mouseOut->setUpdateInterval(5);
    m_mouseOut->setEasingCurve(QEasingCurve::OutCubic);
    m_mouseOut->setEndFrame(minGrey);
    connect(m_mouseOut, &QTimeLine::frameChanged,
            this, &KNMusicListView::changeBackground);
}

void KNMusicListView::resetHeader()
{
    KNMusicLibraryListview::resetHeader();
    setColumnWidth(KNMusicGlobal::Name, 430);
    setColumnWidth(KNMusicGlobal::Time, 60);
    setColumnWidth(KNMusicGlobal::Artist, 125);
    setColumnWidth(KNMusicGlobal::Album, 200);
}

void KNMusicListView::enterEvent(QEvent *e)
{
    m_mouseOut->stop();
    m_mouseIn->stop();
    m_mouseIn->setStartFrame(m_backgroundColor.red());
    m_mouseIn->start();
    KNMusicLibraryListview::enterEvent(e);
}

void KNMusicListView::leaveEvent(QEvent *e)
{
    m_mouseIn->stop();
    m_mouseOut->stop();
    m_mouseOut->setStartFrame(m_backgroundColor.red());
    m_mouseOut->start();
    KNMusicLibraryListview::leaveEvent(e);
}

void KNMusicListView::changeBackground(int frameData)
{
    m_backgroundColor=QColor(frameData, frameData, frameData);
    m_palette.setColor(QPalette::AlternateBase, m_backgroundColor);
    m_palette.setColor(QPalette::Button, m_backgroundColor);
    int textParam=(frameData<<1)+159;
    m_palette.setColor(QPalette::Text, QColor(textParam,
                                              textParam,
                                              textParam,
                                              0xCF+frameData));
    setPalette(m_palette);
}
