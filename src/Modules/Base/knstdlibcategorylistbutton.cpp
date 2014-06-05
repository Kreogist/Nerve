#include <QTimeLine>
#include <QMouseEvent>

#include "knstdlibcategorylistbutton.h"

KNStdLibCategoryListButton::KNStdLibCategoryListButton(QWidget *parent) :
    QLabel(parent)
{
    setAutoFillBackground(true);
    setContentsMargins(10,10,10,10);
    setScaledContents(true);
    setMaximumSize(100, 100);
    m_palette=palette();
    m_palette.setColor(QPalette::Window, m_backgroundColor);
    setPalette(m_palette);

    m_mouseIn=new QTimeLine(100, this);
    m_mouseIn->setUpdateInterval(5);
    m_mouseIn->setEndFrame(100);
    m_mouseIn->setEasingCurve(QEasingCurve::OutCubic);
    connect(m_mouseIn, &QTimeLine::frameChanged,
            this, &KNStdLibCategoryListButton::changeBackground);

    m_mouseOut=new QTimeLine(100, this);
    m_mouseOut->setUpdateInterval(5);
    m_mouseOut->setEndFrame(0);
    m_mouseOut->setEasingCurve(QEasingCurve::OutCubic);
    connect(m_mouseOut, &QTimeLine::frameChanged,
            this, &KNStdLibCategoryListButton::changeBackground);
}

void KNStdLibCategoryListButton::enterEvent(QEvent *event)
{
    QLabel::enterEvent(event);
    m_mouseOut->stop();
    m_mouseIn->setStartFrame(m_backgroundColor.alpha());
    m_mouseIn->start();
}

void KNStdLibCategoryListButton::leaveEvent(QEvent *event)
{
    QLabel::leaveEvent(event);
    m_mouseIn->stop();
    m_mouseOut->setStartFrame(m_backgroundColor.alpha());
    m_mouseOut->start();
}

void KNStdLibCategoryListButton::mousePressEvent(QMouseEvent *event)
{
    QLabel::mousePressEvent(event);
    m_isPressed=true;
}

void KNStdLibCategoryListButton::mouseReleaseEvent(QMouseEvent *event)
{
    QLabel::mouseReleaseEvent(event);
    if(m_isPressed)
    {
        m_isPressed=false;
        emit clicked();
    }
}

void KNStdLibCategoryListButton::changeBackground(const int &frame)
{
    m_backgroundColor.setAlpha(frame);
    m_palette.setColor(QPalette::Window, m_backgroundColor);
    setPalette(m_palette);
}
