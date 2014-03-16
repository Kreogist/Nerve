
#include <QBoxLayout>
#include <QEvent>
#include <QGraphicsOpacityEffect>
#include <QLabel>
#include <QMouseEvent>
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
    setFixedHeight(30);

    //Get palette.
    m_pal=palette();
    m_pal.setColor(QPalette::Window, m_backcolor);
    setPalette(m_pal);

    //Set layout
    QBoxLayout *mainLayout=new QBoxLayout(QBoxLayout::LeftToRight, this);
    mainLayout->setContentsMargins(0,0,0,0);
    mainLayout->setSpacing(0);
    mainLayout->setAlignment(Qt::AlignCenter);
    setLayout(mainLayout);

    //Init labels.
    m_icon->setFixedSize(20, 20);
    m_icon->setScaledContents(true);
    mainLayout->addWidget(m_icon);

    m_iconOpacity=new QGraphicsOpacityEffect(this);
    m_iconOpacity->setOpacity(1.0);
    m_icon->setGraphicsEffect(m_iconOpacity);

    m_caption->setAlignment(Qt::AlignCenter);
    QPalette captionPal=m_caption->palette();
    captionPal.setColor(QPalette::WindowText, QColor(255,255,255));
    m_caption->setPalette(captionPal);
    m_caption->move(0, -height());

    m_captionFont=m_caption->font();
    m_defaultFontSize=m_captionFont.pointSize();
    m_pressedFontSize=m_defaultFontSize-2;

    //Init animes.
    m_mouseInAnime=new QTimeLine(animeDuration, this);
    m_mouseInAnime->setUpdateInterval(2);
    m_mouseInAnime->setEndFrame(0);
    m_mouseInAnime->setEasingCurve(QEasingCurve::OutCubic);
    connect(m_mouseInAnime, SIGNAL(frameChanged(int)),
            this, SLOT(moveCaption(int)));
    connect(m_mouseInAnime, SIGNAL(valueChanged(qreal)),
            this, SLOT(mouseInAnime(qreal)));

    m_mouseOutAnime=new QTimeLine(animeDuration, this);
    m_mouseOutAnime->setUpdateInterval(2);
    m_mouseOutAnime->setEndFrame(-height());
    m_mouseOutAnime->setEasingCurve(QEasingCurve::OutCubic);
    connect(m_mouseOutAnime, SIGNAL(frameChanged(int)),
            this, SLOT(moveCaption(int)));
    connect(m_mouseOutAnime, SIGNAL(valueChanged(qreal)),
            this, SLOT(mouseOutAnime(qreal)));

    m_mouseDownAnime=new QTimeLine(textDuration, this);
    m_mouseDownAnime->setUpdateInterval(1);
    m_mouseDownAnime->setEndFrame(m_pressedFontSize);
    connect(m_mouseDownAnime, SIGNAL(frameChanged(int)),
            this, SLOT(setCaptionFontSize(int)));

    m_mouseUpAnime=new QTimeLine(textDuration, this);
    m_mouseUpAnime->setUpdateInterval(1);
    m_mouseUpAnime->setEndFrame(m_defaultFontSize);
    connect(m_mouseUpAnime, SIGNAL(frameChanged(int)),
            this, SLOT(setCaptionFontSize(int)));
}

void KNStdLibViewerCategoryButton::resizeEvent(QResizeEvent *event)
{
    QWidget::resizeEvent(event);
    m_caption->setFixedWidth(width());
}

void KNStdLibViewerCategoryButton::enterEvent(QEvent *event)
{
    QWidget::enterEvent(event);
    if(!m_checked)
    {
        showTextAnime();
    }
}

void KNStdLibViewerCategoryButton::leaveEvent(QEvent *event)
{
    QWidget::leaveEvent(event);
    if(!m_checked)
    {
        hideTextAnime();
    }
}

void KNStdLibViewerCategoryButton::mousePressEvent(QMouseEvent *event)
{
    KNLibViewerCategoryButton::mousePressEvent(event);
    if(!m_checked)
    {
        startMouseDownAnime();
        m_mousePressed=true;
    }
}

void KNStdLibViewerCategoryButton::mouseMoveEvent(QMouseEvent *event)
{
    KNLibViewerCategoryButton::mouseMoveEvent(event);
    if(!m_checked)
    {
        if(event->buttons()==0x1)
        {
            if(rect().contains(event->pos()))
            {
                startMouseDownAnime();
                m_mousePressed=true;
                return;
            }
            startMouseUPAnime();
            m_mousePressed=false;
        }
    }
}

void KNStdLibViewerCategoryButton::mouseReleaseEvent(QMouseEvent *event)
{
    KNLibViewerCategoryButton::mouseReleaseEvent(event);
    if(!m_checked)
    {
        if(m_mousePressed)
        {
            startMouseUPAnime();
            emit clicked();
        }
    }
}

void KNStdLibViewerCategoryButton::onCheckedChanged()
{
    if(m_checked)
    {
        moveCaption(0);
        mouseInAnime(1.0);
    }
    else
    {
        hideTextAnime();
    }
}

void KNStdLibViewerCategoryButton::moveCaption(int frame)
{
    m_caption->move(0, frame);
}

void KNStdLibViewerCategoryButton::setCaptionFontSize(int pointSize)
{
    m_captionFont.setPointSize(pointSize);
    m_caption->setFont(m_captionFont);
}

void KNStdLibViewerCategoryButton::mouseInAnime(qreal value)
{
    m_iconOpacity->setOpacity(1-value);
    m_backcolor.setAlphaF(value);
    m_pal.setColor(QPalette::Window, m_backcolor);
    setPalette(m_pal);
}

void KNStdLibViewerCategoryButton::mouseOutAnime(qreal value)
{
    m_iconOpacity->setOpacity(value);
    m_backcolor.setAlpha(1-value);
    m_pal.setColor(QPalette::Window, m_backcolor);
    setPalette(m_pal);
}

void KNStdLibViewerCategoryButton::showTextAnime()
{
    m_mouseOutAnime->stop();
    if(m_mouseInAnime->state()!=QTimeLine::Running)
    {
        m_mouseInAnime->setStartFrame(m_caption->pos().y());
        m_mouseInAnime->start();
    }
}

void KNStdLibViewerCategoryButton::hideTextAnime()
{
    m_mouseInAnime->stop();
    if(m_mouseOutAnime->state()!=QTimeLine::Running)
    {
        m_mouseOutAnime->setStartFrame(m_caption->pos().y());
        m_mouseOutAnime->start();
    }
}

void KNStdLibViewerCategoryButton::startMouseDownAnime()
{
    m_mouseUpAnime->stop();
    if(m_mouseDownAnime->state()!=QTimeLine::Running)
    {
        m_mouseDownAnime->setStartFrame(m_caption->font().pointSize());
        m_mouseDownAnime->start();
    }
}

void KNStdLibViewerCategoryButton::startMouseUPAnime()
{
    m_mouseDownAnime->stop();
    if(m_mouseUpAnime->state()!=QTimeLine::Running)
    {
        m_mouseUpAnime->setStartFrame(m_caption->font().pointSize());
        m_mouseUpAnime->start();
    }
}
