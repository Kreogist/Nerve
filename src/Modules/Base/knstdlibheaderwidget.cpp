#include <QTimeLine>

#include "knstdlibheaderwidget.h"

KNStdLibHeaderWidget::KNStdLibHeaderWidget(QWidget *parent) :
    KNLibHeaderWidget(parent)
{
    setAutoFillBackground(true);

    int minGrey=0x10;
    m_backgroundColor=QColor(minGrey, minGrey, minGrey);
    m_palette=palette();
    m_palette.setColor(QPalette::Base, m_backgroundColor);
    m_palette.setColor(QPalette::Window, QColor(0x10, 0x10, 0x10));
    m_palette.setColor(QPalette::Button, QColor(0x10, 0x10, 0x10));
    m_palette.setColor(QPalette::Text, QColor(0x9f, 0x9f, 0x9f));
    setPalette(m_palette);

    m_mouseIn=new QTimeLine(200, this);
    m_mouseIn->setUpdateInterval(5);
    m_mouseIn->setEndFrame(0x50);
    connect(m_mouseIn, &QTimeLine::frameChanged,
            this, &KNStdLibHeaderWidget::changeBackground);

    m_mouseOut=new QTimeLine(200, this);
    m_mouseOut->setUpdateInterval(5);
    m_mouseOut->setEndFrame(minGrey);
    connect(m_mouseOut, &QTimeLine::frameChanged,
            this, &KNStdLibHeaderWidget::changeBackground);
}

void KNStdLibHeaderWidget::enterEvent(QEvent *e)
{
    m_mouseOut->stop();
    m_mouseIn->stop();
    m_mouseIn->setStartFrame(m_backgroundColor.red());
    m_mouseIn->start();
    KNLibHeaderWidget::enterEvent(e);
}

void KNStdLibHeaderWidget::leaveEvent(QEvent *e)
{
    m_mouseIn->stop();
    m_mouseOut->stop();
    m_mouseOut->setStartFrame(m_backgroundColor.red());
    m_mouseOut->start();
    KNLibHeaderWidget::leaveEvent(e);
}

void KNStdLibHeaderWidget::changeBackground(int frameData)
{
    m_backgroundColor=QColor(frameData, frameData, frameData);
    m_palette.setColor(QPalette::Window, m_backgroundColor);
    setPalette(m_palette);
}
