#include <QResizeEvent>

#include "knlibheaderswitcher.h"

KNLibHeaderSwitcher::KNLibHeaderSwitcher(QWidget *parent) :
    QWidget(parent)
{
    setContentsMargins(0,0,0,0);
    setFixedHeight(60);
}

void KNLibHeaderSwitcher::addWidget(QWidget *widget)
{
    if(!m_stackedWidgets.isEmpty())
    {
        widget->setVisible(false);
    }
    m_stackedWidgets.append(widget);
    widget->setParent(this);
    widget->resize(size().width(), size().height()+10);
}

void KNLibHeaderSwitcher::setCurrentIndex(const int &index)
{
    m_currentIndex=index;
}

int KNLibHeaderSwitcher::currentIndex() const
{
    return m_currentIndex;
}

int KNLibHeaderSwitcher::widgetCount() const
{
    return m_stackedWidgets.size();
}

QWidget *KNLibHeaderSwitcher::widgetAt(const int &index)
{
    return m_stackedWidgets.at(index);
}

void KNLibHeaderSwitcher::resizeEvent(QResizeEvent *event)
{
    QWidget::resizeEvent(event);
    QWidget *widget=m_stackedWidgets.at(m_currentIndex);
    widget->resize(event->size().width(), event->size().height()+10);
}
