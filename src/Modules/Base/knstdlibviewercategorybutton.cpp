#include <QEvent>
#include <QPainter>
#include <QMouseEvent>
#include <QPaintEvent>
#include <QResizeEvent>
#include <QTimeLine>

#include <QDebug>

#include "knstdlibviewercategorybutton.h"

KNStdLibViewerCategoryButton::KNStdLibViewerCategoryButton(QWidget *parent) :
    KNLibViewerCategoryButton(parent)
{
    //Set properties.
    setAutoFillBackground(true);
    setContentsMargins(0,0,0,0);
    setFixedHeight(m_fixedHeight);

    //Set default checked.
    m_checked=false;
    //Set icon position.
    m_iconCenterX=(width()-m_iconSize)>>1;
    //Set background gradient.
    m_background=QLinearGradient(0,0,0,height());
    m_background.setColorAt(0, m_gradientColor);
    m_background.setColorAt(1, m_gradientColor);

    //Initial the mouse-in animation.
    m_mouseIn=new QTimeLine(200, this);
    m_mouseIn->setUpdateInterval(2);
    m_mouseIn->setEndFrame(100);
    m_mouseIn->setEasingCurve(QEasingCurve::OutCubic);
    connect(m_mouseIn, &QTimeLine::frameChanged,
            this, &KNStdLibViewerCategoryButton::onActionMouseInOutFrameChange);

    //Initial the mouse-out animation.
    m_mouseOut=new QTimeLine(200, this);
    m_mouseOut->setUpdateInterval(2);
    m_mouseOut->setEndFrame(0);
    m_mouseOut->setEasingCurve(QEasingCurve::OutCubic);
    connect(m_mouseOut, &QTimeLine::frameChanged,
            this, &KNStdLibViewerCategoryButton::onActionMouseInOutFrameChange);

    //Initial the mouse-down animation.
    m_mouseDown=new QTimeLine(200, this);
    m_mouseDown->setUpdateInterval(2);
    m_mouseDown->setEndFrame(200);
    m_mouseDown->setEasingCurve(QEasingCurve::OutCubic);
    connect(m_mouseDown, &QTimeLine::frameChanged,
            this, &KNStdLibViewerCategoryButton::onActionMousePressedChange);

    //Initial the mouse-up animation.
    m_mouseUp=new QTimeLine(200, this);
    m_mouseUp->setUpdateInterval(2);
    m_mouseUp->setEndFrame(255);
    m_mouseUp->setEasingCurve(QEasingCurve::OutCubic);
    connect(m_mouseUp, &QTimeLine::frameChanged,
            this, &KNStdLibViewerCategoryButton::onActionMousePressedChange);
}

void KNStdLibViewerCategoryButton::resizeEvent(QResizeEvent *event)
{
    QWidget::resizeEvent(event);
    m_iconCenterX=(width()-m_iconSize)>>1;
    update();
}

void KNStdLibViewerCategoryButton::enterEvent(QEvent *event)
{
    QWidget::enterEvent(event);
    if(!m_checked)
    {
        //Show Mouse Enter State
        m_mouseOut->stop();
        m_mouseIn->setStartFrame(m_highLightColor.alpha());
        m_mouseIn->start();
    }
}

void KNStdLibViewerCategoryButton::leaveEvent(QEvent *event)
{
    QWidget::leaveEvent(event);
    if(!m_checked)
    {
        startMouseOutAnime();
    }
}

void KNStdLibViewerCategoryButton::mousePressEvent(QMouseEvent *event)
{
    KNLibViewerCategoryButton::mousePressEvent(event);
    if(!m_checked)
    {
        //Show Mouse Down State
        m_mouseUp->stop();
        m_mouseDown->setStartFrame(m_highLightColor.alpha());
        m_mouseDown->start();
        m_mousePressed=true;
    }
}

void KNStdLibViewerCategoryButton::mouseMoveEvent(QMouseEvent *event)
{
    KNLibViewerCategoryButton::mouseMoveEvent(event);
    //Here's pressed check.
    ;
}

void KNStdLibViewerCategoryButton::mouseReleaseEvent(QMouseEvent *event)
{
    KNLibViewerCategoryButton::mouseReleaseEvent(event);
    if(!m_checked)
    {
        //Check if the mouse has been pressed before, avoid other released.
        if(m_mousePressed)
        {
            startMouseUpAnime();
            emit clicked();
        }
    }
}

void KNStdLibViewerCategoryButton::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);
    QPainter painter(this);
    painter.setPen(Qt::NoPen);
    //Draw background
    painter.setBrush(m_background);
    painter.drawRect(rect());
    //Draw top line
    painter.setBrush(m_highLightColor);
    painter.drawRect(0,0,width(),2);
    //Draw icon
    painter.setOpacity(m_iconAlpha);
    painter.drawPixmap(m_iconCenterX,
                       m_iconY,
                       m_iconSize,
                       m_iconSize,
                       m_icon);
    //Draw text
    painter.setPen(QColor(255,255,255));
    painter.setOpacity(m_textAlpha);
    painter.drawText(0,m_textY,width(), height(),
                     Qt::AlignHCenter,
                     m_text);
}

void KNStdLibViewerCategoryButton::onCheckedChanged()
{
    if(m_checked)
    {
        m_textAlpha=1.0;
        m_iconAlpha=0.0;
        m_textY=7;
        startMouseUpAnime();
    }
    else
    {
        startMouseOutAnime();
    }
}

void KNStdLibViewerCategoryButton::onActionMouseInOutFrameChange(const int &frame)
{
    m_highLightColor.setAlpha(frame);
    m_gradientColor.setAlpha(frame>>1);
    m_background.setColorAt(0, m_gradientColor);
    m_textAlpha=(qreal)frame/100;
    m_iconAlpha=1.0-m_textAlpha;
    m_textY=-93+frame;
    update();
}

void KNStdLibViewerCategoryButton::onActionMousePressedChange(const int &frame)
{
    m_highLightColor.setAlpha(frame);
    m_gradientColor.setAlpha(frame>>1);
    m_background.setColorAt(0, m_gradientColor);
    update();
}

void KNStdLibViewerCategoryButton::startMouseOutAnime()
{
    //Show Normal State
    m_mouseDown->stop();
    m_mouseUp->stop();
    m_mouseIn->stop();
    if(m_mouseOut->state()!=QTimeLine::Running)
    {
        m_mouseOut->setStartFrame(m_highLightColor.alpha());
        m_mouseOut->start();
    }
}

void KNStdLibViewerCategoryButton::startMouseUpAnime()
{
    //Stop all other animations.
    m_mouseOut->stop();
    m_mouseIn->stop();
    m_mouseDown->stop();
    if(m_mouseUp->state()!=QTimeLine::Running)
    {
        m_mouseUp->setStartFrame(m_highLightColor.alpha());
        m_mouseUp->start();
    }
}

QString KNStdLibViewerCategoryButton::text() const
{
    return m_text;
}

void KNStdLibViewerCategoryButton::setText(const QString &text)
{
    m_text=text;
}

QPixmap KNStdLibViewerCategoryButton::icon() const
{
    return m_icon;
}

void KNStdLibViewerCategoryButton::setIcon(const QPixmap &icon)
{
    m_icon=icon.scaled(m_iconSize,m_iconSize,
                       Qt::IgnoreAspectRatio,
                       Qt::SmoothTransformation);
}

