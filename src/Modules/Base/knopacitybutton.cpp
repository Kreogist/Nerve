#include <QPropertyAnimation>
#include <QMouseEvent>
#include <QGraphicsOpacityEffect>

#include "knopacitybutton.h"

KNOpacityButton::KNOpacityButton(QWidget *parent) :
    QLabel(parent)
{
    setScaledContents(true);
    m_effect=new QGraphicsOpacityEffect(this);
    m_effect->setOpacity(0.4);
    setGraphicsEffect(m_effect);
    m_mouseDown=new QPropertyAnimation(m_effect, "opacity", this);
    m_mouseDown->setDuration(100);
    m_mouseDown->setEasingCurve(QEasingCurve::OutCubic);
    m_mouseDown->setEndValue(1.0);

    m_mouseUp=new QPropertyAnimation(m_effect, "opacity", this);
    m_mouseUp->setDuration(100);
    m_mouseUp->setEasingCurve(QEasingCurve::OutCubic);
    m_mouseUp->setEndValue(0.8);

    m_mouseIn=new QPropertyAnimation(m_effect, "opacity", this);
    m_mouseIn->setDuration(100);
    m_mouseIn->setEasingCurve(QEasingCurve::OutCubic);
    m_mouseIn->setEndValue(0.8);

    m_mouseOut=new QPropertyAnimation(m_effect, "opacity", this);
    m_mouseOut->setDuration(100);
    m_mouseOut->setEasingCurve(QEasingCurve::OutCubic);
    m_mouseOut->setEndValue(0.4);
}

void KNOpacityButton::enterEvent(QEvent *event)
{
    QWidget::enterEvent(event);
    m_mouseOut->stop();
    m_mouseIn->setStartValue(m_effect->opacity());
    m_mouseIn->start();
}

void KNOpacityButton::leaveEvent(QEvent *event)
{
    QWidget::leaveEvent(event);
    m_mouseIn->stop();
    m_mouseOut->setStartValue(m_effect->opacity());
    m_mouseOut->start();
}

void KNOpacityButton::mousePressEvent(QMouseEvent *event)
{
    QLabel::mousePressEvent(event);
    emit mousePressed();
    m_mouseUp->stop();
    m_mouseDown->setStartValue(m_effect->opacity());
    m_mouseDown->start();
    m_isPressed=true;
}

void KNOpacityButton::mouseReleaseEvent(QMouseEvent *event)
{
    QLabel::mouseReleaseEvent(event);
    emit mouseReleased();
    m_mouseDown->stop();
    if(m_isPressed && rect().contains(event->pos()))
    {
        m_mouseUp->setStartValue(m_effect->opacity());
        m_mouseUp->start();
        emit clicked();
    }
    m_isPressed=false;
}
