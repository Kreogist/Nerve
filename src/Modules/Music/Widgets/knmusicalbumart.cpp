#include <QResizeEvent>
#include <QPaintEvent>
#include <QMouseEvent>
#include <QPainter>
#include <QLinearGradient>

#include "knmusicalbumart.h"

KNMusicAlbumArt::KNMusicAlbumArt(QWidget *parent) :
    QLabel(parent)
{
    m_highLight=QLinearGradient(QPointF(0,0), QPointF(0, height()));
    m_highLight.setColorAt(0, QColor(0xff,0xff,0xff,130));
    m_highLight.setColorAt(1, QColor(0xff,0xff,0xff,0));

    m_highLightArea[0]=QPointF(0,0);
    m_highLightArea[1]=QPointF((float)width()*0.7, 0);
    m_highLightArea[2]=QPointF(0, (float)height());
}

void KNMusicAlbumArt::mousePressEvent(QMouseEvent *event)
{
    QLabel::mousePressEvent(event);
    m_isPressed=true;
}

void KNMusicAlbumArt::mouseReleaseEvent(QMouseEvent *event)
{
    QLabel::mouseReleaseEvent(event);
    if(m_isPressed && rect().contains(event->pos()))
    {
        if(event->button()==Qt::LeftButton)
        {
            m_isPressed=false;
            if(m_isPlayerShown)
            {
                m_isPlayerShown=false;
                emit requireHideMusicPlayer();
            }
            else
            {
                m_isPlayerShown=true;
                emit requireShowMusicPlayer();
            }
        }
    }
}

void KNMusicAlbumArt::paintEvent(QPaintEvent *event)
{
    QLabel::paintEvent(event);
    QPainter painter(this);
    painter.setPen(Qt::NoPen);
    painter.setRenderHint(QPainter::Antialiasing, true);
    painter.setRenderHint(QPainter::SmoothPixmapTransform, true);
    painter.setBrush(m_highLight);
    painter.drawPolygon(m_highLightArea, 3);
}

void KNMusicAlbumArt::resizeEvent(QResizeEvent *event)
{
    QLabel::resizeEvent(event);
    m_highLight.setFinalStop(0, height());
    m_highLightArea[1]=QPointF((float)width()*0.7, 0);
    m_highLightArea[2]=QPointF(0, (float)height());
}
