#include <QEvent>
#include <QMouseEvent>
#include <QPaintEvent>
#include <QResizeEvent>

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
    m_palette=palette();
    m_palette.setColor(QPalette::Window, m_backcolor);
    setPalette(m_palette);

    ;
}

void KNStdLibViewerCategoryButton::resizeEvent(QResizeEvent *event)
{
    QWidget::resizeEvent(event);
}

void KNStdLibViewerCategoryButton::enterEvent(QEvent *event)
{
    QWidget::enterEvent(event);
    if(!m_checked)
    {
        //Show Mouse Enter State
    }
}

void KNStdLibViewerCategoryButton::leaveEvent(QEvent *event)
{
    QWidget::leaveEvent(event);
    if(!m_checked)
    {
        //Show Normal State
    }
}

void KNStdLibViewerCategoryButton::mousePressEvent(QMouseEvent *event)
{
    KNLibViewerCategoryButton::mousePressEvent(event);
    if(!m_checked)
    {
        //Show Mouse Down State
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
        //
        ;
    }
}

void KNStdLibViewerCategoryButton::paintEvent(QPaintEvent *event)
{
    ;
}

QString KNStdLibViewerCategoryButton::text() const
{
    return m_text;
}

void KNStdLibViewerCategoryButton::setText(const QString &text)
{
    m_text = text;
}

QPixmap KNStdLibViewerCategoryButton::icon() const
{
    return m_icon;
}

void KNStdLibViewerCategoryButton::setIcon(const QPixmap &icon)
{
    m_icon = icon;
}

