#include <QEvent>
#include <QTimeLine>
#include <QPainter>
#include <QPaintEvent>
#include <QDebug>

#include "knplayerprogress.h"

KNPlayerProgress::KNPlayerProgress(QWidget *parent) :
    QAbstractSlider(parent)
{
    m_palette=palette();
    m_window=QColor(255,255,255,40);
    m_palette.setColor(QPalette::Window, m_window);
    m_button=QColor(255,255,255,90);
    m_palette.setColor(QPalette::Button, m_button);

    m_mouseIn=new QTimeLine(400, this);
    m_mouseIn->setEasingCurve(QEasingCurve::OutCubic);
    m_mouseIn->setEndFrame(70);
    connect(m_mouseIn, SIGNAL(frameChanged(int)),
            this, SLOT(onActionChangeColor(int)));

    m_mouseOut=new QTimeLine(400, this);
    m_mouseOut->setEasingCurve(QEasingCurve::OutCubic);
    m_mouseOut->setEndFrame(40);
    connect(m_mouseOut, SIGNAL(frameChanged(int)),
            this, SLOT(onActionChangeColor(int)));
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
    QAbstractSlider::enterEvent(event);
    m_mouseOut->stop();
    m_mouseIn->setStartFrame(m_window.alpha());
    m_mouseIn->start();
}

void KNPlayerProgress::leaveEvent(QEvent *event)
{
    QAbstractSlider::leaveEvent(event);
    m_mouseIn->stop();
    m_mouseOut->setStartFrame(m_window.alpha());
    m_mouseOut->start();
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
    int top=(height()-m_scrollHeight)>>1;
    painter.drawRect(0, top, width()-1, m_scrollHeight);
    double percent=
            ((double)value()-(double)minimum())/((double)maximum()-(double)minimum()),
           valueWidthF=percent*(double)width();
    int valueWidth=(int)valueWidthF;
    painter.setPen(Qt::NoPen);
    painter.setBrush(QBrush(m_button));
    painter.drawRect(1, top+1, valueWidth-1, m_scrollHeight-2);
}

void KNPlayerProgress::onActionChangeColor(const int &time)
{
    m_window.setAlpha(time);
    m_button.setAlpha((time<<1)+10);
    update();
}
