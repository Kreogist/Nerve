#include <QMouseEvent>
#include <QDebug>

#include "knabstractslider.h"

KNAbstractSlider::KNAbstractSlider(QWidget *parent) :
    QWidget(parent)
{
    ;
}

float KNAbstractSlider::minimum() const
{
    return m_minimal;
}

void KNAbstractSlider::setMinimum(float minimal)
{
    m_minimal = minimal;
    if(m_value<m_minimal)
    {
        setValue(m_minimal);
    }
    m_range=m_maximum-m_minimal;
    emit rangeChanged(m_minimal, m_maximum);
}

float KNAbstractSlider::maximum() const
{
    return m_maximum;
}

void KNAbstractSlider::setMaximum(float maximum)
{
    m_maximum = maximum;
    if(m_value>m_maximum)
    {
        setValue(m_maximum);
    }
    m_range=m_maximum-m_minimal;
    emit rangeChanged(m_minimal, m_maximum);
}

float KNAbstractSlider::value() const
{
    return m_value;
}

void KNAbstractSlider::mousePressEvent(QMouseEvent *event)
{
    m_isSliderDown=true;
    emit sliderPressed();
    QWidget::mousePressEvent(event);
}

void KNAbstractSlider::mouseReleaseEvent(QMouseEvent *event)
{
    m_isSliderDown=false;
    emit sliderReleased();
    QWidget::mouseReleaseEvent(event);
}

bool KNAbstractSlider::isSliderDown() const
{
    return m_isSliderDown;
}

void KNAbstractSlider::setIsSliderDown(bool isSliderDown)
{
    m_isSliderDown = isSliderDown;
}

void KNAbstractSlider::setValue(float value)
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

void KNAbstractSlider::setRange(float min, float max)
{
    m_minimal=min;
    m_maximum=max;
    if(m_value>m_maximum)
    {
        setValue(m_maximum);
    }
    if(m_value<m_minimal)
    {
        setValue(m_minimal);
    }
    m_range=m_maximum-m_minimal;
    emit rangeChanged(min, max);
}
