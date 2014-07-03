#include <QPaintEvent>
#include <QPainter>
#include <QDebug>

#include <math.h>

#include "knmusicvisualeffect.h"

KNMusicVisualEffect::KNMusicVisualEffect(QWidget *parent) :
    QWidget(parent)
{
    setSizePolicy(QSizePolicy::MinimumExpanding,
                  QSizePolicy::MinimumExpanding);
}

float *KNMusicVisualEffect::fftData()
{
    return m_fft;
}

void KNMusicVisualEffect::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);
    QPainter painter(this);
    int b0=0,i,y,b1, bandWidth=width()/20,bandCount=20, currentX=0;
    float peak, heightF=(float)(height());
    painter.setPen(Qt::NoPen);
    painter.setBrush(m_itemColor);
    for(i=0;i<bandCount;i++)
    {
        peak=0;
        b1=pow(2,i*11.0/(bandCount-1));
        //If the start frame is larger than the top bound, then stop it.
        if(b1>2047)
        {
            b1=2047;
        }
        // make sure it uses at least 1 FFT bin
        if(b1<=b0)
        {
            b1=b0+1;
        }
        //Find the biggest fft for this frame.
        for (;b0<b1;b0++)
            if (peak<m_fft[1+b0])
                peak=m_fft[1+b0];
        y=qMax(1.0+log10f(peak)/3.0, 0.0)*heightF;
        painter.drawRect(currentX,
                         height()-y,
                         bandWidth,
                         y);
        if(2047==b1)
        {
            break;
        }
        currentX+=bandWidth+1;
    }
}
