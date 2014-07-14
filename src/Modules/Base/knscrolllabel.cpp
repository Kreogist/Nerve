#include <QSizePolicy>
#include <QPainter>
#include <QPaintEvent>
#include <QTimer>

#include <QDebug>

#include "knscrolllabel.h"

KNScrollLabel::KNScrollLabel(QWidget *parent) :
    QWidget(parent)
{
    //Set properties.
    setFixedHeight(fontMetrics().height());
    setSizePolicy(QSizePolicy::MinimumExpanding,
                  QSizePolicy::Fixed);

    //Initial moving Anime.
    m_movingAnime=new QTimer(this);
    m_movingAnime->setInterval(50);
    connect(m_movingAnime, SIGNAL(timeout()),
            this, SLOT(onActionMoving()));

    m_waiting=new QTimer(this);
    m_waiting->setInterval(5000);
    m_waiting->setSingleShot(true);
    connect(m_waiting, SIGNAL(timeout()),
            this, SLOT(onActionWatingFinished()));
}

QString KNScrollLabel::text() const
{
    return m_text;
}

void KNScrollLabel::setText(const QString &text)
{
    //Stop timers before.
    stopAllTimer();
    //Set texts and values.
    m_text=text;
    m_textX=0;
    //Update viewport.
    update();
    //If text is so long, enable timer.
    int textWidth=fontMetrics().width(m_text);
    if(textWidth>width())
    {
        m_leftMostX=width()-textWidth;
        m_waiting->start();
    }
    else
    {
        m_leftMostX=0;
    }
}

void KNScrollLabel::paintEvent(QPaintEvent *event)
{
    QWidget::paintEvent(event);
    QPainter painter(this);
    painter.drawText(m_textX, 0,
                     fontMetrics().width(m_text), fontMetrics().height(),
                     Qt::AlignVCenter,
                     m_text);
}

void KNScrollLabel::onActionMoving()
{
    if(m_movingLeft)
    {
        if(m_textX<=m_leftMostX)
        {
            m_movingLeft=false;
            m_movingAnime->stop();
            m_waiting->setInterval(2500);
            m_waiting->start();
            return;
        }
        m_textX--;
        update();
        return;
    }
    if(m_textX>=0)
    {
        m_movingLeft=true;
        m_movingAnime->stop();
        m_waiting->setInterval(5000);
        m_waiting->start();
        return;
    }
    m_textX++;
    update();
}

void KNScrollLabel::onActionWatingFinished()
{
    m_waiting->stop();
    m_movingAnime->start();
}

void KNScrollLabel::stopAllTimer()
{
    m_waiting->stop();
    m_movingAnime->stop();
}
