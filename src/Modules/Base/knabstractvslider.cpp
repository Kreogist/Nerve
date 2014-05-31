#include <QMouseEvent>
#include <QDebug>

#include "knabstractvslider.h"

KNAbstractVSlider::KNAbstractVSlider(QWidget *parent) :
    QWidget(parent)
{
    ;
}

float KNAbstractVSlider::minimal() const
{
    return m_minimal;
}

void KNAbstractVSlider::setMinimal(float minimal)
{
    m_minimal = minimal;
    m_range=m_maximum-m_minimal;
    emit rangeChanged(m_minimal, m_maximum);
}

float KNAbstractVSlider::maximum() const
{
    return m_maximum;
}

void KNAbstractVSlider::setMaximum(float maximum)
{
    m_maximum = maximum;
    m_range=m_maximum-m_minimal;
    emit rangeChanged(m_minimal, m_maximum);
}

float KNAbstractVSlider::value() const
{
    return m_value;
}

void KNAbstractVSlider::mousePressEvent(QMouseEvent *event)
{
    m_isSliderDown=true;
    emit sliderPressed();
    QWidget::mousePressEvent(event);
}

void KNAbstractVSlider::mouseReleaseEvent(QMouseEvent *event)
{
    m_isSliderDown=false;
    emit sliderReleased();
    QWidget::mouseReleaseEvent(event);
}

bool KNAbstractVSlider::isSliderDown() const
{
    return m_isSliderDown;
}

void KNAbstractVSlider::setIsSliderDown(bool isSliderDown)
{
    m_isSliderDown = isSliderDown;
}

void KNAbstractVSlider::setValue(float value)
{
    m_value=value;
    if(m_value>m_maximum)
    {
        m_value=m_maximum;
    }
    if(m_minimal>m_value)
    {
        m_value=m_minimal;
    }
    emit valueChanged(m_value);
}

void KNAbstractVSlider::setRange(float min, float max)
{
    m_minimal=min;
    m_maximum=max;
    m_range=m_maximum-m_minimal;
    emit rangeChanged(min, max);
}



