#include <QPropertyAnimation>
#include <QParallelAnimationGroup>

#include <QDebug>

#include "knstdlibheaderswitcher.h"

KNStdLibHeaderSwitcherWidget::KNStdLibHeaderSwitcherWidget(QWidget *parent) :
    KNLibHeaderSwitcherWidget(parent)
{
    m_moveOut=new QPropertyAnimation(this);
    m_moveOut->setPropertyName("geometry");
    m_moveOut->setEasingCurve(QEasingCurve::OutCubic);
    m_moveIn=new QPropertyAnimation(this);
    m_moveIn->setPropertyName("geometry");
    m_moveIn->setEasingCurve(QEasingCurve::OutCubic);
    m_movedGroup=new QParallelAnimationGroup(this);
    m_movedGroup->addAnimation(m_moveOut);
    m_movedGroup->addAnimation(m_moveIn);
    connect(m_movedGroup, &QParallelAnimationGroup::finished,
            this, &KNStdLibHeaderSwitcherWidget::onActionAnimationComplete);
}

void KNStdLibHeaderSwitcherWidget::setCurrentIndex(const int &index)
{
    if(index==currentIndex())
    {
        return;
    }
    m_aboutToBeCurrentIndex=index;
    QWidget *fromWidget=widgetAt(currentIndex()),
            *toWidget=widgetAt(m_aboutToBeCurrentIndex);
    fromWidget->raise();
    m_moveOut->setTargetObject(fromWidget);
    m_moveIn->setTargetObject(toWidget);
    QRect centerPosition=geometry(),
          outPostion=QRect(0,
                           -height(),
                           width(),
                           height());
    m_moveOut->setStartValue(centerPosition);
    m_moveOut->setEndValue(outPostion);
    m_moveIn->setStartValue(outPostion);
    m_moveIn->setEndValue(centerPosition);
    toWidget->setGeometry(outPostion);
    toWidget->setVisible(true);
    m_movedGroup->start();
}

void KNStdLibHeaderSwitcherWidget::onActionAnimationComplete()
{
    KNLibHeaderSwitcherWidget::setCurrentIndex(m_aboutToBeCurrentIndex);
}

KNStdLibHeaderSwitcher::KNStdLibHeaderSwitcher(QWidget *parent) :
    KNLibHeaderSwitcher(parent)
{
    m_swicher=new KNStdLibHeaderSwitcherWidget(this);
    setSwitcher(m_swicher);
}
