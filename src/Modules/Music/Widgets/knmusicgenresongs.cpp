#include <QTimeLine>
#include <QHeaderView>

#include "../knmusicglobal.h"

#include "knmusicgenresongs.h"

KNMusicGenreSongs::KNMusicGenreSongs(QWidget *parent) :
    KNMusicLibraryListview(parent)
{
    setAutoFillBackground(false);
    m_alternateColor=QColor(0xff, 0xff, 0xff, 0);
    m_palette=palette();
    m_palette.setColor(QPalette::Base, QColor(0,0,0,0));
    m_palette.setColor(QPalette::AlternateBase, QColor(0xff, 0xff, 0xff, 0));
    m_palette.setColor(QPalette::Window, QColor(0x20, 0x20, 0x20));
    m_palette.setColor(QPalette::Button, QColor(0x20, 0x20, 0x20));
    m_palette.setColor(QPalette::Text, QColor(0x9f, 0x9f, 0x9f));
    m_palette.setColor(QPalette::Highlight, QColor(0x60, 0x60, 0x60));
    m_palette.setColor(QPalette::HighlightedText, QColor(0xf7, 0xcf, 0x3d));
    setPalette(m_palette);

    m_mouseIn=new QTimeLine(200, this);
    m_mouseIn->setUpdateInterval(5);
    m_mouseIn->setEasingCurve(QEasingCurve::OutCubic);
    m_mouseIn->setEndFrame(0x20);
    connect(m_mouseIn, &QTimeLine::frameChanged,
            this, &KNMusicGenreSongs::changeBackground);

    m_mouseOut=new QTimeLine(200, this);
    m_mouseOut->setUpdateInterval(5);
    m_mouseOut->setEasingCurve(QEasingCurve::OutCubic);
    m_mouseOut->setEndFrame(0);
    connect(m_mouseOut, &QTimeLine::frameChanged,
            this, &KNMusicGenreSongs::changeBackground);
}

void KNMusicGenreSongs::resetHeader()
{
    connect(header(), &QHeaderView::sortIndicatorChanged,
            this, &KNMusicGenreSongs::onActionSort);
    for(int i=KNMusicGlobal::Name+1;
        i<KNMusicGlobal::MusicDataCount;
        i++)
    {
        setColumnHidden(i, true);
    }
    setColumnHidden(KNMusicGlobal::Time, false);
    setColumnHidden(KNMusicGlobal::Album, false);
    setColumnHidden(KNMusicGlobal::Artist, false);
    setColumnHidden(KNMusicGlobal::Rating, false);
    moveToFirst(KNMusicGlobal::Rating);
    moveToFirst(KNMusicGlobal::Album);
    moveToFirst(KNMusicGlobal::Artist);
    moveToFirst(KNMusicGlobal::Time);
    moveToFirst(KNMusicGlobal::Name);
}

void KNMusicGenreSongs::enterEvent(QEvent *e)
{
    m_mouseOut->stop();
    m_mouseIn->stop();
    m_mouseIn->setStartFrame(m_alternateColor.alpha());
    m_mouseIn->start();
    KNMusicLibraryListview::enterEvent(e);
}

void KNMusicGenreSongs::leaveEvent(QEvent *e)
{
    m_mouseIn->stop();
    m_mouseOut->stop();
    m_mouseOut->setStartFrame(m_alternateColor.alpha());
    m_mouseOut->start();
    KNMusicLibraryListview::leaveEvent(e);
}

void KNMusicGenreSongs::changeBackground(int frameData)
{
    m_alternateColor.setAlpha(frameData);
    m_palette.setColor(QPalette::AlternateBase, m_alternateColor);
    m_palette.setColor(QPalette::Button, QColor(0x20+frameData,
                                                0x20+frameData,
                                                0x20+frameData));
    int textParam=(frameData<<1)+159;
    m_palette.setColor(QPalette::Text, QColor(textParam,
                                              textParam,
                                              textParam,
                                              0xDF+frameData));
    setPalette(m_palette);
}
