#include <QPropertyAnimation>
#include <QShowEvent>

#include "knmenu.h"

KNMenu::KNMenu(QWidget *parent) :
    QMenu(parent)
{
    m_showAnimation=new QPropertyAnimation(this, "geometry", this);
    m_showAnimation->setEasingCurve(QEasingCurve::OutCubic);
}

void KNMenu::showEvent(QShowEvent *e)
{
    QRect endPosition=geometry();
    QRect startPostion=endPosition;
    startPostion.setHeight(0);
    m_showAnimation->setStartValue(startPostion);
    m_showAnimation->setEndValue(endPosition);
    m_showAnimation->start();
    QMenu::showEvent(e);
}


