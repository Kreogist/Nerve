#include <QBoxLayout>

#include "knstdlibviewercategory.h"
#include "knwidgetswitcher.h"
#include "knlibsearcher.h"

#include <QLabel>

#include "knstdlibviewer.h"

KNStdLibViewer::KNStdLibViewer(QWidget *parent) :
    KNLibViewer(parent)
{
    setContentsMargins(0,0,0,0);

    m_layout=new QBoxLayout(QBoxLayout::TopToBottom, this);
    m_layout->setContentsMargins(0,0,0,0);
    m_layout->setSpacing(0);
    setLayout(m_layout);

    m_category=new KNStdLibViewerCategory(this);
    m_layout->addWidget(m_category);

    m_listview=new KNWidgetSwitcher(this);
    m_layout->addWidget(m_listview);

    connect(m_category, &KNStdLibViewerCategory::categoryChanged,
            m_listview, &KNWidgetSwitcher::setCurrentIndex);
}

void KNStdLibViewer::addCategory(const QPixmap &icon,
                                 const QString &category,
                                 QWidget *widget)
{
    m_category->addCategory(icon, category);
    m_listview->addWidget(widget);
}

void KNStdLibViewer::setCategoryIndex(const int &index)
{
    m_category->setCurrentIndex(index);
}

void KNStdLibViewer::setSearcher(KNLibSearcher *searcher)
{
    m_searcher=searcher;
}

void KNStdLibViewer::moveLeft()
{
    m_category->moveLeft();
}

void KNStdLibViewer::moveRight()
{
    m_category->moveRight();
}
