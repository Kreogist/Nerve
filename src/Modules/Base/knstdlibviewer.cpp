#include <QAction>
#include <QBoxLayout>
#include <QLabel>
#include <QKeySequence>

#include <QDebug>

#include "knstdlibviewercategory.h"
#include "knwidgetswitcher.h"
#include "knlibsearcher.h"

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
    m_category->installEventFilter(this);
    m_layout->addWidget(m_category);

    m_widgetSwicher=new KNWidgetSwitcher(this);
    m_widgetSwicher->installEventFilter(this);
    m_layout->addWidget(m_widgetSwicher);

    connect(m_category, &KNStdLibViewerCategory::categoryChanged,
            m_widgetSwicher, &KNWidgetSwitcher::setCurrentIndex);
    connect(m_widgetSwicher, &KNWidgetSwitcher::movedComplete,
            m_category, &KNStdLibViewerCategory::enabledMoving);

    QAction *leftShortcut=new QAction(this);
    leftShortcut->setShortcut(QKeySequence::Back);
    connect(leftShortcut, SIGNAL(triggered()),
            m_category, SLOT(moveLeft()));
    addAction(leftShortcut);

    QAction *rightShortcut=new QAction(this);
    rightShortcut->setShortcut(QKeySequence::Forward);
    connect(rightShortcut, SIGNAL(triggered()),
            m_category, SLOT(moveRight()));
    addAction(rightShortcut);
}

void KNStdLibViewer::addCategory(const QPixmap &icon,
                                 const QString &category,
                                 QWidget *widget)
{
    m_category->addCategory(icon, category);
    m_widgetSwicher->addWidget(widget);
}

void KNStdLibViewer::setCategoryIndex(const int &index)
{
    m_category->setCurrentIndex(index);
}

int KNStdLibViewer::categoryIndex() const
{
    return m_category->currentIndex();
}

void KNStdLibViewer::moveLeft()
{
    m_category->moveLeft();
}

void KNStdLibViewer::moveRight()
{
    m_category->moveRight();
}

void KNStdLibViewer::setContentsFocus()
{
    m_widgetSwicher->setWidgetFocus(m_widgetSwicher->currentIndex());
}
