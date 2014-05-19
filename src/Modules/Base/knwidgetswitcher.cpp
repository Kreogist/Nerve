#include <QPropertyAnimation>
#include <QParallelAnimationGroup>
#include <QResizeEvent>

#include <QDebug>

#include "knwidgetswitcher.h"

KNWidgetSwitcher::KNWidgetSwitcher(QWidget *parent) :
    QWidget(parent)
{
    setContentsMargins(0,0,0,0);
    m_switchAnime=new QParallelAnimationGroup(this);

    m_flyOutAnime=new QPropertyAnimation(this);
    m_flyOutAnime->setPropertyName("geometry");
    m_flyOutAnime->setEasingCurve(QEasingCurve::OutCubic);
    m_flyOutAnime->setDuration(200);

    m_flyInAnime=new QPropertyAnimation(this);
    m_flyInAnime->setPropertyName("geometry");
    m_flyInAnime->setEasingCurve(QEasingCurve::OutCubic);
    m_flyInAnime->setDuration(200);

    m_switchAnime->addAnimation(m_flyInAnime);
    m_switchAnime->addAnimation(m_flyOutAnime);
    connect(m_switchAnime, &QParallelAnimationGroup::finished,
            this, &KNWidgetSwitcher::hideMovedOut);
}

void KNWidgetSwitcher::addWidget(QWidget *widget)
{
    widget->setParent(this);
    widget->installEventFilter(this);
    if(m_widgets.size()==0)
    {
        m_currentIndex=0;
        widget->setVisible(true);
        widget->setEnabled(true);
    }
    else
    {
        widget->setVisible(false);
        widget->setEnabled(false);
    }
    m_widgets.append(widget);
}

int KNWidgetSwitcher::currentIndex() const
{
    return m_currentIndex;
}

int KNWidgetSwitcher::size() const
{
    return m_widgets.size();
}

void KNWidgetSwitcher::setCurrentIndex(int currentIndex)
{
    if(currentIndex<m_widgets.size())
    {
        m_switchAnime->stop();
        m_flyOutAnime->setTargetObject(m_widgets.at(m_currentIndex));
        m_flyInAnime->setTargetObject(m_widgets.at(currentIndex));
        QRect centerRect=rect(),
              leftRect=QRect(-centerRect.width(),
                             0,
                             centerRect.width(),
                             centerRect.height()),
              rightRect=QRect(centerRect.width(),
                              0,
                              centerRect.width(),
                              centerRect.height());
        m_flyOutAnime->setStartValue(centerRect);
        m_flyInAnime->setEndValue(centerRect);
        if(currentIndex > m_currentIndex)
        {
            //This widget is after the current, move left.
            m_flyOutAnime->setEndValue(leftRect);
            m_flyInAnime->setStartValue(rightRect);
        }
        else
        {
            //Move right.
            m_flyOutAnime->setEndValue(rightRect);
            m_flyInAnime->setStartValue(leftRect);
        }
        m_movedOutPage = m_currentIndex;
        setWidgetVisible(currentIndex, true);
        setWidgetFocus(currentIndex);
        m_switchAnime->start();
        m_currentIndex = currentIndex;
    }
}

void KNWidgetSwitcher::resizeEvent(QResizeEvent *event)
{
    QWidget::resizeEvent(event);
    for(QList<QWidget *>::iterator i=m_widgets.begin(),
                                   finalWidget=m_widgets.end();
        i!=finalWidget;
        ++i)
    {
        (*i)->resize(event->size());
    }
}

void KNWidgetSwitcher::hideMovedOut()
{
    setWidgetVisible(m_movedOutPage, false);
    emit movedComplete();
}

void KNWidgetSwitcher::setWidgetVisible(const int &index,
                                        const bool &visible)
{
    if(index < m_widgets.size())
    {
        QWidget *indexWidget=m_widgets.at(index);
        indexWidget->setEnabled(visible);
        indexWidget->setVisible(visible);
    }
}

void KNWidgetSwitcher::setWidgetFocus(const int &index)
{
    m_widgets.at(index)->setFocus();
}

