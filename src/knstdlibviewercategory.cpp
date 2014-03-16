#include <QBoxLayout>
#include <QLinearGradient>
#include <QPaintEvent>
#include <QPainter>
#include <QSignalMapper>

#include <QDebug>

#include "knstdlibviewercategory.h"

KNStdLibViewerCategory::KNStdLibViewerCategory(QWidget *parent) :
    KNLibViewerCategory(parent)
{
    //Set properties.
    setAutoFillBackground(true);
    setContentsMargins(0,0,0,0);

    m_layout=new QBoxLayout(QBoxLayout::LeftToRight, this);
    m_layout->setContentsMargins(0,0,0,0);
    m_layout->setSpacing(0);
    setLayout(m_layout);

    QPalette pal=palette();
    m_backgroundColor=QColor(0x53,0x53,0x53);
    pal.setColor(QPalette::Window, m_backgroundColor);
    setPalette(pal);

    m_categoryClicked=new QSignalMapper(this);
    connect(m_categoryClicked, SIGNAL(mapped(int)),
            this, SLOT(categoryClicked(int)));
}

void KNStdLibViewerCategory::addCategory(const QPixmap &icon,
                                         const QString &category)
{
    KNStdLibViewerCategoryButton *button=new KNStdLibViewerCategoryButton(this);
    button->setIcon(icon);
    button->setText(category);
    connect(button, SIGNAL(clicked()),
            m_categoryClicked, SLOT(map()));
    m_categoryClicked->setMapping(button, m_categories.count());
    if(m_currentCategory==-1)
    {
        m_currentCategory=0;
        button->setChecked(true);
    }
    m_layout->addWidget(button);
    m_categories.append(button);
}

void KNStdLibViewerCategory::setText(const int &index,
                                     const QString &text)
{
    if(index<m_categories.count())
    {
        KNStdLibViewerCategoryButton *button=m_categories.at(index);
        button->setText(text);
    }
}

void KNStdLibViewerCategory::paintEvent(QPaintEvent *event)
{
    KNLibViewerCategory::paintEvent(event);
}

void KNStdLibViewerCategory::categoryClicked(int index)
{
    if(m_currentCategory!=-1 && m_currentCategory<m_categories.count())
    {
        KNStdLibViewerCategoryButton *button=m_categories.at(m_currentCategory);
        button->setChecked(false);
    }
    if(index!=-1 && index<m_categories.count())
    {
        m_currentCategory=index;
        KNStdLibViewerCategoryButton *button=m_categories.at(m_currentCategory);
        button->setChecked(true);
        emit categoryChanged(m_currentCategory);
    }
}
