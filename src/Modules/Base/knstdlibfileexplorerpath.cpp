#include <QBoxLayout>

#include "knstdlibfileexplorerpath.h"

KNStdLibFileExplorerPath::KNStdLibFileExplorerPath(QWidget *parent) :
    KNLibFileExplorerPath(parent)
{
    //Set properties.
    setContentsMargins(0,0,0,0);

    //Set layout.
    m_mainLayout=new QBoxLayout(QBoxLayout::LeftToRight, this);
    m_mainLayout->setContentsMargins(0,0,0,0);
    m_mainLayout->setSpacing(0);
    setLayout(m_mainLayout);
}
