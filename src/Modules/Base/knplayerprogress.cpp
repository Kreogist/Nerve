#include <QEvent>
#include <QTimeLine>
#include <QMouseEvent>
#include <QPainter>
#include <QWheelEvent>
#include <QPaintEvent>
#include <QDebug>

#include "knplayerprogress.h"

KNPlayerProgress::KNPlayerProgress(QWidget *parent) :
    KNAbstractSlider(parent)
{
    setMinimumHeight(20);
    m_buttonGradient.setCenterRadius(8);
    m_buttonGradient.setColorAt(1, QColor(0,0,0,0));

    m_palette=palette();
    m_window=QColor(255,255,255,40);
    m_palette.setColor(QPalette::Window, m_window);
    m_button=QColor(255,255,255,90);
    m_palette.setColor(QPalette::Button, m_button);

    m_mouseIn=new QTimeLine(400, this);
    m_mouseIn->setEasingCurve(QEasingCurve::OutCubic);
    m_mouseIn->setEndFrame(70);
    connect(m_mouseIn, &QTimeLine::frameChanged,
            this, &KNPlayerProgress::onActionChangeColor);

    m_mouseOut=new QTimeLine(400, this);
    m_mouseOut->setEasingCurve(QEasingCurve::OutCubic);
    m_mouseOut->setEndFrame(40);
    connect(m_mouseOut, &QTimeLine::frameChanged,
            this, &KNPlayerProgress::onActionChangeColor);
    connect(this, SIGNAL(valueChanged(float)),
            this, SLOT(update()));
}

const QPalette &KNPlayerProgress::palette() const
{
    return m_palette;
}

void KNPlayerProgress::setPalette(const QPalette &pal)
{
    m_palette=pal;
}

void KNPlayerProgress::enterEvent(QEvent *event)
{
    m_mouseOut->stop();
    m_mouseIn->setStartFrame(m_window.alpha());
    m_mouseIn->start();
    KNAbstractSlider::enterEvent(event);
}

void KNPlayerProgress::leaveEvent(QEvent *event)
{
    m_mouseIn->stop();
    m_mouseOut->setStartFrame(m_window.alpha());
    m_mouseOut->start();
    KNAbstractSlider::leaveEvent(event);
}

void KNPlayerProgress::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    if(autoFillBackground())
    {
        painter.setPen(Qt::NoPen);
        painter.setBrush(QBrush(m_palette.color(QPalette::Window)));
        painter.drawRect(event->rect());
    }
    QColor sideColor=m_palette.color(QPalette::WindowText);
    sideColor.setAlpha(100);
    QPen pen=QPen(sideColor);
    pen.setStyle(Qt::SolidLine);
    painter.setPen(pen);
    painter.setBrush(QBrush(m_window));
    int top=(event->rect().height()-m_scrollHeight)>>1;
    painter.drawRect(0, top, width()-1, m_scrollHeight);
    float percent=(value()-minimum())/(maximum()-minimum()),
           valueWidthF=percent*(float)width();
    int valueWidth=(int)valueWidthF;
    painter.setPen(Qt::NoPen);
    painter.setBrush(QBrush(m_button));
    painter.drawRect(1, top+1, valueWidth-1, m_scrollHeight-1);

    //Paint the button.
    int center=top+(m_scrollHeight>>1)+1;
    m_buttonGradient.setCenter(QPointF(valueWidth, center));
    m_buttonGradient.setFocalPoint(QPointF(valueWidth, center));
    m_buttonGradient.setColorAt(0, QColor(255,255,255,105+m_button.alpha()));
    painter.setBrush(m_buttonGradient);
    painter.drawRoundRect(QRect(valueWidth-16,
                                top+1,
                                16,
                                m_scrollHeight-1));
}

void KNPlayerProgress::mousePressEvent(QMouseEvent *event)
{
    emit sliderPressed();
    setValue((maximum()-minimum())/(float)width()*(float)event->pos().x()+
             minimum());
    m_mouseDown=true;
    event->accept();
}

void KNPlayerProgress::mouseMoveEvent(QMouseEvent *event)
{
    if(m_mouseDown)
    {
        setValue((maximum()-minimum())/(float)width()*(float)event->pos().x()
                 +minimum());
    }
}

void KNPlayerProgress::mouseReleaseEvent(QMouseEvent *event)
{
    emit sliderReleased();
    m_mouseDown=false;
    event->accept();
}

void KNPlayerProgress::wheelEvent(QWheelEvent *event)
{
    KNAbstractSlider::wheelEvent(event);
    if(event->pixelDelta()==QPoint(0,0))
    {
        QPoint dataPoint=event->angleDelta()/40;
        setValue(dataPoint.x()==0?
                     value()+(dataPoint.y()):
                     value()+(dataPoint.x()));
    }
    else
    {
        setValue(event->pixelDelta().x()==0?
                     value()+event->pixelDelta().y():
                     value()+event->pixelDelta().x());
    }
    emit sliderReleased();
}

void KNPlayerProgress::onActionChangeColor(const int &time)
{
    m_window.setAlpha(time);
    m_button.setAlpha((time<<1)+10);
    update();
}
