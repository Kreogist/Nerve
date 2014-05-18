#include <QResizeEvent>

#include "knlibheaderswitcher.h"

KNLibHeaderSwitcherWidget::KNLibHeaderSwitcherWidget(QWidget *parent) :
    QWidget(parent)
{
    setFixedHeight(60);
}

void KNLibHeaderSwitcherWidget::addWidget(QWidget *widget)
{
    if(!m_stackedWidgets.isEmpty())
    {
        widget->setVisible(false);
    }
    m_stackedWidgets.append(widget);
    widget->setParent(this);
    widget->resize(size());
}

void KNLibHeaderSwitcherWidget::setCurrentIndex(const int &index)
{
    m_currentIndex=index;
}

int KNLibHeaderSwitcherWidget::currentIndex() const
{
    return m_currentIndex;
}

int KNLibHeaderSwitcherWidget::widgetCount() const
{
    return m_stackedWidgets.size();
}

QWidget *KNLibHeaderSwitcherWidget::widgetAt(const int &index)
{
    return m_stackedWidgets.at(index);
}

void KNLibHeaderSwitcherWidget::resizeEvent(QResizeEvent *event)
{
    QWidget::resizeEvent(event);
    QWidget *widget=m_stackedWidgets.at(m_currentIndex);
    widget->resize(event->size());
}

KNLibHeaderSwitcher::KNLibHeaderSwitcher(QWidget *parent) :
    QDockWidget(parent)
{
    QWidget *emptyHack=new QWidget(this);
    setTitleBarWidget(emptyHack);
}

void KNLibHeaderSwitcher::setSwitcher(KNLibHeaderSwitcherWidget *switcher)
{
    setWidget(switcher);
    m_headerSwitcherWidget=switcher;
}

void KNLibHeaderSwitcher::addWidget(QWidget *widget)
{
    m_headerSwitcherWidget->addWidget(widget);
}

void KNLibHeaderSwitcher::setCurrentIndex(const int &index)
{
    m_headerSwitcherWidget->setCurrentIndex(index);
}

int KNLibHeaderSwitcher::currentIndex() const
{
    return m_headerSwitcherWidget->currentIndex();
}

int KNLibHeaderSwitcher::widgetCount() const
{
    return m_headerSwitcherWidget->widgetCount();
}
