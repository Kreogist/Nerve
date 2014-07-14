#include <QBoxLayout>

#include "knstatusbar.h"

KNStatusBar::KNStatusBar(QWidget *parent) :
    KNLibStatusBar(parent)
{
    //Set properties.
    setContentsMargins(0,0,0,0);

    //Set layout.
    m_mainLayout=new QBoxLayout(QBoxLayout::LeftToRight, this);
    m_mainLayout->setContentsMargins(5,5,5,5);
    m_mainLayout->setSpacing(0);
    setLayout(m_mainLayout);

    //Add default widgets.
    ;
}
