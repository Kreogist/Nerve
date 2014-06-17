#include <QPainter>
#include <QPaintEvent>
#include <QMouseEvent>

#include <QDebug>

#include "knmusicvolumeslider.h"

KNMusicVolumeSlider::KNMusicVolumeSlider(QWidget *parent) :
    KNAbstractSlider(parent)
{
    //Initial all the pixmaps.
    m_sliderBase=QPixmap(":/Music/Resources/Music/EQs/hsliderbase.png");
    m_sliderBaseLeft=QPixmap(":/Music/Resources/Music/EQs/hsliderbaseleft.png");
    m_sliderBaseRight=QPixmap(":/Music/Resources/Music/EQs/hsliderbaseright.png");
    m_sliderButton=QPixmap(":/Music/Resources/Music/EQs/hsliderbutton.png");
    m_kopieLeft=QPixmap(":/Music/Resources/Music/EQs/hsliderLeft.png");
    m_kopieMid=QPixmap(":/Music/Resources/Music/EQs/hsliderMid.png");
    m_kopieRight=QPixmap(":/Music/Resources/Music/EQs/hsliderRight.png");
    setMinimumHeight(m_sliderButton.height());
    m_leftKopieMargin=m_leftKopieStart+m_kopieLeft.width()-1;
    m_leftRange=m_sliderBase.width();
    m_leftMargin=m_sliderBaseLeft.width();
    m_rightMargin=m_sliderBase.width()+m_leftMargin;
    m_mouseRange=m_rightMargin-m_leftMargin;
    connect(this, &KNAbstractSlider::rangeChanged,
            [=]
            {
                updateButtonPosition();
                update();
    });
}

void KNMusicVolumeSlider::setValue(float value)
{
    KNAbstractSlider::setValue(value);
    updateButtonPosition();
    update();
}

void KNMusicVolumeSlider::paintEvent(QPaintEvent *event)
{
    KNAbstractSlider::paintEvent(event);
    QPainter painter(this);
    int sliderY=(height()-m_sliderBase.height())>>1,
        sliderKopieY=((height()-m_kopieMid.height())>>1)+1;
    //Draw the base of slider.
    painter.drawPixmap(0,
                       sliderY,
                       m_sliderBaseLeft.width(),
                       m_sliderBaseLeft.height(),
                       m_sliderBaseLeft);
    painter.drawPixmap(m_leftMargin,
                       sliderY,
                       m_mouseRange,
                       m_sliderBase.height(),
                       m_sliderBase);
    painter.drawPixmap(m_rightMargin,
                       sliderY,
                       m_sliderBaseRight.width(),
                       m_sliderBaseRight.height(),
                       m_sliderBaseRight);
    //Draw kopie.
    painter.drawPixmap(m_leftKopieStart,
                       sliderKopieY,
                       m_kopieLeft.width(),
                       m_kopieLeft.height(),
                       m_kopieLeft);
    if(m_buttonLeft>m_leftKopieMargin)
    {
        painter.drawPixmap(m_leftKopieMargin,
                           sliderKopieY,
                           m_buttonLeft,
                           m_kopieMid.height(),
                           m_kopieMid);
    }
    painter.drawPixmap(m_buttonLeft,
                       ((height()-m_sliderButton.height())>>1),
                       m_sliderButton.width(),
                       m_sliderButton.height(),
                       m_sliderButton);
}

void KNMusicVolumeSlider::resizeEvent(QResizeEvent *event)
{
    KNAbstractSlider::resizeEvent(event);
    m_rightMargin=width()-m_sliderButton.width();
    m_mouseRange=m_rightMargin-m_leftMargin;
    updateButtonPosition();
}

void KNMusicVolumeSlider::mouseMoveEvent(QMouseEvent *event)
{
    if(m_isSliderDown)
    {
        setValueFromMouseParam((float)event->x());
        emit sliderMoved(m_value);
    }
}

void KNMusicVolumeSlider::mouseReleaseEvent(QMouseEvent *event)
{
    if(m_isSliderDown)
    {
        setValueFromMouseParam((float)event->x());
        emit sliderMoved(m_value);
    }
    KNAbstractSlider::mouseReleaseEvent(event);
}

void KNMusicVolumeSlider::wheelEvent(QWheelEvent *event)
{
    KNAbstractSlider::wheelEvent(event);
    if(event->pixelDelta()==QPoint(0,0))
    {
        setValue(event->angleDelta().x()==0?
                     value()+(event->angleDelta().y()<<1):
                     value()+(event->angleDelta().x()<<1));
        return;
    }
    setValue(event->pixelDelta().x()==0?
                 value()+(event->pixelDelta().y()<<7):
                 value()+(event->pixelDelta().x()<<7));
}

void KNMusicVolumeSlider::updateButtonPosition()
{
    m_buttonLeft=m_value/m_range*m_mouseRange+
            m_leftMargin-(m_sliderButton.width()>>1);
}

void KNMusicVolumeSlider::setValueFromMouseParam(float mouseParam)
{
    if(mouseParam<m_leftMargin) mouseParam=m_leftMargin;
    if(mouseParam>m_rightMargin) mouseParam=m_rightMargin;
    //This is a hack.
    m_buttonLeft=mouseParam-m_leftMargin;
    setValue((mouseParam-m_leftMargin)/m_mouseRange*m_range);
    update();
}
