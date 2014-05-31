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
    QPainter painter(this);
    int SPECWIDTH=width(), b0=0,x,y,b1,BANDS=20, bandWidth=SPECWIDTH/BANDS, currentX=0;
#define SPECHEIGHT 70
    float peak;
    painter.setPen(Qt::NoPen);
    painter.setBrush(m_itemColor);
    for(x=0;x<BANDS;x++)
    {
        peak=0;
        b1=pow(2,x*10.0/(BANDS-1));
        if (b1>1023) b1=1023;
        if (b1<=b0) b1=b0+1; // make sure it uses at least 1 FFT bin
        for (;b0<b1;b0++)
            if (peak<m_fft[1+b0])
                peak=m_fft[1+b0];
        y=qsqrt(peak)*3*SPECHEIGHT-4; // scale it (sqrt to make low values more visible)
        if (y>SPECHEIGHT) y=SPECHEIGHT; // cap it
        painter.drawRect(currentX,
                         71-y,
                         bandWidth,
                         y);
        currentX+=bandWidth;
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
    return 1/y;
}
