#include <QPaintEvent>
#include <QPainter>
#include <QDebug>

#include <math.h>

#include "knmusicvisualeffect.h"

KNMusicVisualEffect::KNMusicVisualEffect(QWidget *parent) :
    QWidget(parent)
{
    ;
}

float *KNMusicVisualEffect::fftData()
{
    return m_fft;
}

void KNMusicVisualEffect::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    int SPECWIDTH=width(), b0=0,x,y,b1,BANDS=20, bandWidth=SPECWIDTH/BANDS, currentX=0;
    float peak;
    painter.setPen(Qt::NoPen);
    painter.setBrush(QColor(90,90,90));
    for(x=0;x<BANDS;x++)
    {
        peak=0;
        b1=pow(2,x*10.0/(BANDS-1));
        if (b1>1023) b1=1023;
        if (b1<b0) b1=b0; // make sure it uses at least 1 FFT bin
        for (;b0<b1;b0++)
            if (peak<m_fft[1+b0])
                peak=m_fft[1+b0];
        y=sqrt(peak)*210-4; // scale it (sqrt to make low values more visible)
        if (y>70) y=70; // cap it
        painter.drawRect(currentX,
                         70-y,
                         bandWidth,
                         y);
        currentX+=bandWidth;
    }
}
