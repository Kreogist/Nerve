#include <QResizeEvent>
#include <QSizePolicy>

#include "knlibheaderswitcher.h"

KNLibHeaderSwitcher::KNLibHeaderSwitcher(QWidget *parent) :
    QWidget(parent)
{
    setContentsMargins(0,0,0,0);
    setFixedHeight(70);
}

void KNLibHeaderSwitcher::addWidget(QWidget *widget)
{
    if(!m_stackedWidgets.isEmpty())
    {
        widget->setVisible(false);
    }
    m_stackedWidgets.append(widget);
    widget->setParent(this);
    widget->setGeometry(0,0,size().width(), size().height());
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

QWidget *KNLibHeaderSwitcher::currentWidget() const
{
    return m_stackedWidgets.at(m_currentIndex);
}

void KNLibHeaderSwitcher::resizeEvent(QResizeEvent *event)
{
    QWidget::resizeEvent(event);
    QWidget *widget=m_stackedWidgets.at(m_currentIndex);
    widget->setGeometry(0,0,event->size().width(), event->size().height());
}
