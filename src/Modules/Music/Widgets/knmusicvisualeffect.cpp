#include <QResizeEvent>
#include <QPaintEvent>
#include <QPainter>
#include <QDebug>

#include <stdlib.h>
#include <math.h>

#include "knmusicvisualeffect.h"

KNMusicVisualEffect::KNMusicVisualEffect(QWidget *parent) :
    QWidget(parent)
{
    memset(m_fft, 0, sizeof(m_fft));
    setSizePolicy(QSizePolicy::MinimumExpanding,
                  QSizePolicy::MinimumExpanding);
}

float *KNMusicVisualEffect::fftData()
{
    return m_fft;
}

void KNMusicVisualEffect::setSampleRate(float sampleRate)
{
    memset(m_maxPoint, 0, sizeof(m_maxPoint));
    memset(m_framePoint, 0, sizeof(m_framePoint));
    memset(m_frameHeight, 1, sizeof(m_frameHeight));
    if(sampleRate!=0)
    {
        for(int i=0; i<20; i++)
        {
            m_framePoint[i]=m_topPoint[i]/sampleRate*2047.0;
            if(m_framePoint[i]>2047)
            {
                m_framePoint[i]=-1;
            }
        }
    }
}

void KNMusicVisualEffect::prepareGraphic()
{
    memset(m_frameHeight, 1, sizeof(m_frameHeight));
    for(int i=0; i<20; i++)
    {
        if(m_framePoint[i]==-1)
        {
            break;
        }
        m_frameHeight[i]=20*log10f(m_fft[m_framePoint[i]])/-70;
    }
}

void KNMusicVisualEffect::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);
    QPainter painter(this);
    int i,y,bandWidth=width()/20, currentX=0;
    painter.setPen(Qt::NoPen);
    painter.setBrush(m_itemColor);
    for(i=0;i<20;i++)
    {
        y=m_frameHeight[i]*height();
        if(y<=m_maxPoint[i] && y>-1)
        {
            m_maxPoint[i]=y;
        }
        else
        {
            m_maxPoint[i]=m_maxPoint[i]<height()?m_maxPoint[i]+2:height();
        }
        painter.drawRect(currentX,
                         m_maxPoint[i],
                         bandWidth,
                         1);
        painter.drawRect(currentX,
                         y,
                         bandWidth,
                         height()-y);
        currentX+=bandWidth+1;
    }
}
