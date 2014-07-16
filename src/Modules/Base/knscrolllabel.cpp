#include <QSizePolicy>
#include <QPainter>
#include <QPaintEvent>
#include <QResizeEvent>
#include <QTimer>
#include <QSizePolicy>

#include <QDebug>

#include "knscrolllabel.h"

KNScrollLabel::KNScrollLabel(QWidget *parent) :
    QWidget(parent)
{
    //Set properties.
    setFixedHeight(fontMetrics().height());
    setSizePolicy(QSizePolicy(QSizePolicy::MinimumExpanding,
                              QSizePolicy::Fixed,
                              QSizePolicy::Label));

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
    //Reset the Timer.
    m_waiting->setInterval(5000);
    //Set texts and values.
    m_text=text;
    m_textX=0;
    //Update viewport.
    update();
    //Update animation parameters.
    updateAnimeParam();
}

void KNScrollLabel::setFont(const QFont &font)
{
    QWidget::setFont(font);
    //Update the height.
    setFixedHeight(fontMetrics().height());
}

void KNScrollLabel::paintEvent(QPaintEvent *event)
{
    //Do the background and other painting.
    QWidget::paintEvent(event);
    //Paint the text.
    QPainter painter(this);
    painter.setFont(font());
    painter.translate(contentsMargins().left(),
                      contentsMargins().top());
    painter.drawText(m_textX, 0,
                     fontMetrics().width(m_text), fontMetrics().height(),
                     Qt::AlignVCenter,
                     m_text);
}

void KNScrollLabel::resizeEvent(QResizeEvent *event)
{
    QWidget::resizeEvent(event);
    //Update the animations.
    updateAnimeParam();
    //Check is it still need to scroll.
    if(m_leftMostX==0)
    {
        //If not then
        stopAllTimer();
        m_textX=0;
        update();
    }
}

void KNScrollLabel::onActionMoving()
{
    //If moving left,
    if(m_movingLeft)
    {
        //Check is it at the most left, if sure.
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

void KNScrollLabel::updateAnimeParam()
{
    //If text is so long, enable timer.
    int textWidth=fontMetrics().width(m_text),
        rectWidth=contentsRect().width();
    if(textWidth>rectWidth)
    {
        stopAllTimer();
        m_leftMostX=rectWidth-textWidth;
        setToolTip(m_text);
        m_waiting->start();
    }
    else
    {
        m_leftMostX=0;
        setToolTip("");
    }
}

void KNScrollLabel::stopAllTimer()
{
    m_waiting->stop();
    m_movingAnime->stop();
}
