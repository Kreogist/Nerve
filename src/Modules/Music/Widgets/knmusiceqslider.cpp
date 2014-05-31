#include <QPainter>
#include <QPaintEvent>
#include <QMouseEvent>

#include <QDebug>

#include "knmusiceqslider.h"

KNMusicEQSlider::KNMusicEQSlider(QWidget *parent) :
    KNAbstractVSlider(parent)
{
    //Initial all the pixmaps.
    m_sliderBase=QPixmap(":/Music/Resources/Music/EQs/sliderbase.png");
    m_sliderButton=QPixmap(":/Music/Resources/Music/EQs/sliderbutton.png");
    m_topRange=m_sliderBase.height()-m_sliderButton.height();
    m_topMargin=m_sliderButton.height()>>1;
    m_bottomMargin=m_sliderBase.height()-m_topMargin;
    m_mouseRange=m_bottomMargin-m_topMargin;
    connect(this, SIGNAL(rangeChanged(float,float)),
            this, SLOT(updateButtonPosition()));
    setFixedHeight(m_sliderBase.height());
}

void KNMusicEQSlider::setValue(float value)
{
    KNAbstractVSlider::setValue(value);
    updateButtonPosition();
}

void KNMusicEQSlider::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    painter.drawPixmap((width()-m_sliderBase.width())>>1,
                       0,
                       m_sliderBase.width(),
                       m_sliderBase.height(),
                       m_sliderBase);
    painter.drawPixmap(((width()-m_sliderButton.width())>>1)-1,
                       m_buttonTop,
                       m_sliderButton.width(),
                       m_sliderButton.height(),
                       m_sliderButton);
}

void KNMusicEQSlider::mouseMoveEvent(QMouseEvent *event)
{
    if(m_isSliderDown)
    {
        float mouseParam=(float)event->y(),
              rawValue;
        if(mouseParam<m_topMargin) mouseParam=m_topMargin;
        if(mouseParam>m_bottomMargin) mouseParam=m_bottomMargin;
        //This is a hack.
        m_buttonTop=mouseParam-m_topMargin;
        rawValue=(mouseParam-m_topMargin)/m_mouseRange*m_range;
        setValue(m_maximum-rawValue);
        update();
        emit sliderMoved(m_value);
    }
}

void KNMusicEQSlider::updateButtonPosition()
{
    m_buttonTop=(m_maximum-m_value)/m_range*m_mouseRange;
    update();
}
