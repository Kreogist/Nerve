#include <QBoxLayout>
#include <QSignalMapper>
#include <QTimeLine>

#include <QDebug>

#include "knstdlibviewercategory.h"

KNStdLibViewerCategory::KNStdLibViewerCategory(QWidget *parent) :
    KNLibViewerCategory(parent)
{
    //Set properties.
    setAutoFillBackground(true);
    setContentsMargins(0,0,0,0);
    setFixedHeight(30);

    //Set layout
    m_layout=new QBoxLayout(QBoxLayout::LeftToRight, this);
    m_layout->setContentsMargins(0,0,0,0);
    m_layout->setSpacing(0);
    setLayout(m_layout);

    //Set palette.
    int minGrey=0x20;
    m_palette=palette();
    m_backgroundColor=QColor(minGrey, minGrey, minGrey);
    m_palette.setColor(QPalette::Window, m_backgroundColor);
    setPalette(m_palette);

    m_categoryClicked=new QSignalMapper(this);
    connect(m_categoryClicked, SIGNAL(mapped(int)),
            this, SLOT(categoryClicked(int)));

    m_mouseIn=new QTimeLine(200, this);
    m_mouseIn->setUpdateInterval(5);
    m_mouseIn->setEasingCurve(QEasingCurve::InCubic);
    m_mouseIn->setEndFrame(0x40);
    connect(m_mouseIn, &QTimeLine::frameChanged,
            this, &KNStdLibViewerCategory::changeBackground);

    m_mouseOut=new QTimeLine(200, this);
    m_mouseOut->setUpdateInterval(5);
    m_mouseIn->setEasingCurve(QEasingCurve::OutCubic);
    m_mouseOut->setEndFrame(minGrey);
    connect(m_mouseOut, &QTimeLine::frameChanged,
            this, &KNStdLibViewerCategory::changeBackground);
}

void KNStdLibViewerCategory::addCategory(const QPixmap &icon,
                                         const QString &category)
{
    KNStdLibViewerCategoryButton *button=new KNStdLibViewerCategoryButton(this);
    button->setIcon(icon);
    button->setText(category);
    connect(button, SIGNAL(clicked()),
            m_categoryClicked, SLOT(map()));
    m_categoryClicked->setMapping(button, m_categories.size());
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
    if(index<m_categories.size())
    {
        KNStdLibViewerCategoryButton *button=m_categories.at(index);
        button->setText(text);
    }
}

void KNStdLibViewerCategory::setCurrentIndex(const int &index)
{
    if(m_moving)
    {
        return;
    }
    if(m_currentCategory!=-1 && m_currentCategory<m_categories.size())
    {
        KNStdLibViewerCategoryButton *button=m_categories.at(m_currentCategory);
        button->setChecked(false);
    }
    if(index!=-1 && index<m_categories.size())
    {
        m_currentCategory=index;
        KNStdLibViewerCategoryButton *button=m_categories.at(m_currentCategory);
        button->setChecked(true);
        m_moving=true;
        emit categoryChanged(m_currentCategory);
    };
}

void KNStdLibViewerCategory::moveLeft()
{
    if(m_currentCategory==0)
    {
        setCurrentIndex(m_categories.size()-1);
    }
    else
    {
        setCurrentIndex(m_currentCategory-1);
    }
}

void KNStdLibViewerCategory::moveRight()
{
    if(m_currentCategory==m_categories.size()-1)
    {
        setCurrentIndex(0);
    }
    else
    {
        setCurrentIndex(m_currentCategory+1);
    }
}

void KNStdLibViewerCategory::enabledMoving()
{
    m_moving=false;
}

void KNStdLibViewerCategory::enterEvent(QEvent *e)
{
    m_mouseOut->stop();
    m_mouseIn->stop();
    m_mouseIn->setStartFrame(m_backgroundColor.red());
    m_mouseIn->start();
    KNLibViewerCategory::enterEvent(e);
}

void KNStdLibViewerCategory::leaveEvent(QEvent *e)
{
    m_mouseIn->stop();
    m_mouseOut->stop();
    m_mouseOut->setStartFrame(m_backgroundColor.red());
    m_mouseOut->start();
    KNLibViewerCategory::leaveEvent(e);
}

void KNStdLibViewerCategory::categoryClicked(int index)
{
    setCurrentIndex(index);
}

void KNStdLibViewerCategory::changeBackground(int frameData)
{
    m_backgroundColor=QColor(frameData, frameData, frameData);
    m_palette.setColor(QPalette::Window, m_backgroundColor);
    setPalette(m_palette);
}
