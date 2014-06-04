#include <QBoxLayout>

#include "knverticalwidgetswitcher.h"

#include "knstdlibcategoryswitcher.h"

KNStdLibCategorySwitcher::KNStdLibCategorySwitcher(QWidget *parent) :
    KNLibCategorySwitcher(parent)
{
    //Set properties.
    setContentsMargins(0,0,0,0);

    //Set layouts.
    m_layout=new QBoxLayout(QBoxLayout::LeftToRight, this);
    m_layout->setContentsMargins(0,0,0,0);
    m_layout->setSpacing(0);
    setLayout(m_layout);

    m_categoryWidgets=new KNVerticalWidgetSwitcher(this);
    m_layout->addWidget(m_categoryWidgets);
}

void KNStdLibCategorySwitcher::addCategory(const QString &name,
                                           const QPixmap &icon,
                                           QWidget *centralWidget)
{
    emit requireAddCategory(name, icon);
    m_categoryWidgets->addWidget(centralWidget);
}

void KNStdLibCategorySwitcher::setCentralWidgetIndex(const int &index)
{
    m_categoryWidgets->setCurrentIndex(index);
}
