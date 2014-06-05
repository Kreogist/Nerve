#include <QPaintEvent>
#include <QPainter>
#include <QDebug>

#include <math.h>

#include "knmusicvisualeffect.h"

KNMusicVisualEffect::KNMusicVisualEffect(QWidget *parent) :
    QWidget(parent)
{
}

float *KNMusicVisualEffect::fftData()
{
    return m_fft;
}

void KNMusicVisualEffect::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);
    QPainter painter(this);
    int SPECWIDTH=width(), SPECHEIGHT=(height()>>1),
            b0=0,x,y,b1, bandWidth=10,BANDS=SPECWIDTH/(bandWidth+1), currentX=0;
    float peak;
    painter.setPen(Qt::NoPen);
    painter.setBrush(m_itemColor);
    for(x=0;x<BANDS;x++)
    {
        peak=0;
        b1=pow(2,x*10.0/(BANDS-1));
        if (b1>2047) b1=2047;
        if (b1<=b0) b1=b0+1; // make sure it uses at least 1 FFT bin
        for (;b0<b1;b0++)
            if (peak<m_fft[1+b0])
                peak=m_fft[1+b0];
        y=((int)(qsqrt(peak)*SPECHEIGHT)<<1); // scale it (sqrt to make low values more visible)
//        if (y>SPECHEIGHT) y=SPECHEIGHT; // cap it
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

//Refer to quake3-1.32b's source code Q_rsqrt() for fast square root calculate.
float KNMusicVisualEffect::qsqrt(const float &number)
{
    float y=number;
    long i=*(long*) &number;   // evil floating point bit level hacking
    i=0x5f375a86-(i>>1); // what the fuck?
    y=*(float*)&i;
    y*=(1.5F-(number*0.5F*y*y)); // 1st iteration
    //y*=(threehalfs-(x2*y*y));// 2nd iteration, this can be removed
    return qAbs(1/y);
}
