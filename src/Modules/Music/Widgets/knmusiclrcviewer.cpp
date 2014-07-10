#include <QTimeLine>
#include <QDebug>
#include <QResizeEvent>
#include <QPainter>

#include "../Libraries/knmusiclrcparser.h"
#include "../../Public/Base/knmusicbackend.h"

#include "knmusiclrcviewer.h"

KNMusicLRCViewer::KNMusicLRCViewer(QWidget *parent) :
    QWidget(parent)
{
    m_lrcParser=new KNMusicLRCParser(this);
    m_centerAnime=new QTimeLine(100, this);
    m_centerAnime->setUpdateInterval(2);
//    m_centerAnime->setEasingCurve(QEasingCurve::OutCubic);
    connect(m_centerAnime, &QTimeLine::frameChanged,
            [=](const int &frame)
            {
                m_paintingY=frame;
                update();
            });
}

void KNMusicLRCViewer::loadLyrics(const QString &filePath)
{
    //Clear the cache.
    m_positions.clear();
    m_lyrics.clear();
    if(m_lrcParser->readLyrics(filePath))
    {
        //Get all the positions and texts.
        m_positions=m_lrcParser->positions();
        m_lyrics=m_lrcParser->lyricsTexts();
        //Reset viewer.
        resetViewer();
        //Initial the next postion.
        m_nextPosition=1;
        m_centerY=((height()-fontMetrics().height())>>1);
        m_centerAnime->setEndFrame(m_centerY);
        m_paintingY=m_centerY;
    }
    update();
}

void KNMusicLRCViewer::setMusicBackend(KNMusicBackend *player)
{
    connect(player, &KNMusicBackend::positionChanged,
            this, &KNMusicLRCViewer::onActionPositionChanged);
}

void KNMusicLRCViewer::onActionPositionChanged(const float &position)
{
    if(m_positions.isEmpty())
    {
        return;
    }
    int positionLine=m_currentLine;
    while(positionLine < m_positions.size() &&
          position > m_positions.at(positionLine))
    {
        positionLine++;
        m_paintingY+=fontMetrics().height();
    }
    if(positionLine!=0)
    {
        positionLine--;
        m_paintingY-=fontMetrics().height();
    }
    if(positionLine!=m_currentLine)
    {
        m_currentLine=positionLine;
        update();
        if(m_centerAnime->state()==QTimeLine::Running)
        {
            m_centerAnime->stop();
        }
        m_centerAnime->setStartFrame(m_paintingY);
        m_centerAnime->start();
    }
}

void KNMusicLRCViewer::paintEvent(QPaintEvent *event)
{
    QWidget::paintEvent(event);
    QPainter painter(this);
    painter.setRenderHint(QPainter::TextAntialiasing);
    //If there's no lyrics, display other data.
    if(m_lyrics.isEmpty())
    {
        return;
    }
    int lineHeight=fontMetrics().height(), paintLine=m_currentLine+1;
    //Draw the current line.
    painter.setPen(QColor(255,255,255));
    painter.drawText(0,m_paintingY,width(),lineHeight,
                     Qt::AlignLeft,
                     m_lyrics.at(m_currentLine));
    painter.setPen(QColor(100,100,100));
    //Draw the line under the current line.
    int currentLineBottom=m_paintingY+lineHeight;
    while(currentLineBottom<height() && paintLine<m_lyrics.size())
    {
        painter.drawText(0, currentLineBottom, width(), lineHeight,
                         Qt::AlignLeft,
                         m_lyrics.at(paintLine));
        paintLine++;
        currentLineBottom+=lineHeight;
    }
    //Draw the line before the current line.
    currentLineBottom=m_paintingY;
    paintLine=m_currentLine-1;
    while(currentLineBottom>0 && paintLine>0)
    {
        painter.drawText(0, currentLineBottom-lineHeight, width(), lineHeight,
                         Qt::AlignLeft,
                         m_lyrics.at(paintLine));
        paintLine--;
        currentLineBottom-=lineHeight;
    }
}

void KNMusicLRCViewer::resizeEvent(QResizeEvent *event)
{
    QWidget::resizeEvent(event);
    m_centerY=((height()-fontMetrics().height())>>1);
    m_centerAnime->setEndFrame(m_centerY);
}

void KNMusicLRCViewer::resetViewer()
{
    m_currentLine=0;
    m_nextPosition=0;
}
