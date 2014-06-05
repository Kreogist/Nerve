#include <QBoxLayout>
#include <QTimeLine>
#include <QResizeEvent>

#include <QDebug>

#include "knwidgetswitcher.h"

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

    //Set category switcher.
    m_categoryWidgets=new KNWidgetSwitcher(this);
    m_layout->addWidget(m_categoryWidgets);

    //Initial the dummy shadow to disable the operate of the contents.
    m_dummyShadow=new QWidget(this);
    m_dummyShadow->setAutoFillBackground(true);
    m_dummyPalette=m_dummyShadow->palette();
    changeOpacity(0);
    m_dummyShadow->hide();
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

void KNStdLibCategorySwitcher::changeOpacity(const qreal &opacityParam)
{
    m_shadowColor.setAlpha(opacityParam*200.0);
    m_dummyPalette.setColor(QPalette::Window, m_shadowColor);
    m_dummyShadow->setPalette(m_dummyPalette);
}

void KNStdLibCategorySwitcher::enableContent()
{
    m_dummyShadow->hide();
}

void KNStdLibCategorySwitcher::disableContent()
{
    m_dummyShadow->show();
}

void KNStdLibCategorySwitcher::resizeEvent(QResizeEvent *event)
{
    KNLibCategorySwitcher::resizeEvent(event);
    m_dummyShadow->resize(event->size());
}
