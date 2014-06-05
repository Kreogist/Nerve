#include <QPainter>
#include <QPaintEvent>
#include <QLinearGradient>

#include "knsideshadows.h"

KNTopSideShadow::KNTopSideShadow(QWidget *parent) :
    QWidget(parent)
{
    ;
}

void KNTopSideShadow::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    QLinearGradient shadow(QPoint(0,0), rect().bottomLeft());
    shadow.setColorAt(0, QColor(0,0,0,130));
    shadow.setColorAt(1, QColor(0,0,0,0));
    painter.setBrush(shadow);
    painter.drawRect(event->rect().x()-1,
                     event->rect().y()-1,
                     event->rect().width()+1,
                     event->rect().height()+1);
}

KNBottomSideShadow::KNBottomSideShadow(QWidget *parent) :
    QWidget(parent)
{
    ;
}

void KNBottomSideShadow::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    QLinearGradient shadow(rect().bottomLeft(), QPoint(0,0));
    shadow.setColorAt(0, QColor(0,0,0,130));
    shadow.setColorAt(1, QColor(0,0,0,0));
    painter.setBrush(shadow);
    painter.drawRect(event->rect().x()-1,
                     event->rect().y()-1,
                     event->rect().width()+1,
                     event->rect().height()+1);
}

KNRightSideShadow::KNRightSideShadow(QWidget *parent) :
    QWidget(parent)
{
    ;
}

void KNRightSideShadow::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    QLinearGradient shadow(QPoint(0,0), rect().topRight());
    shadow.setColorAt(0, QColor(0,0,0,130));
    shadow.setColorAt(1, QColor(0,0,0,0));
    painter.setBrush(shadow);
    painter.drawRect(event->rect().x()-1,
                     event->rect().y()-1,
                     event->rect().width()+1,
                     event->rect().height()+1);
}

KNLeftSideShadow::KNLeftSideShadow(QWidget *parent) :
    QWidget(parent)
{
    ;
}

void KNLeftSideShadow::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    QLinearGradient shadow(QPoint(0,0), rect().topRight());
    shadow.setColorAt(0, QColor(0,0,0,0));
    shadow.setColorAt(1, QColor(0,0,0,130));
    painter.setBrush(shadow);
    painter.drawRect(event->rect().x()-1,
                     event->rect().y()-1,
                     event->rect().width()+1,
                     event->rect().height()+1);
}
