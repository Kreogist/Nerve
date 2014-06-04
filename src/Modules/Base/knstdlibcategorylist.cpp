#include <QBoxLayout>

#include <QDebug>

#include "knstdlibcategorybutton.h"

#include "knstdlibcategorylist.h"

KNStdLibCategoryList::KNStdLibCategoryList(QWidget *parent) :
    KNLibCategoryList(parent)
{
    setContentsMargins(0,0,0,0);
    m_button=new KNStdLibCategoryButton;

    m_layout=new QBoxLayout(QBoxLayout::LeftToRight, this);
    m_layout->setContentsMargins(0,0,0,0);
    m_layout->setSpacing(0);
    setLayout(m_layout);
}

KNStdLibCategoryList::~KNStdLibCategoryList()
{
    if(m_button->parent()==0)
    {
        m_button->deleteLater();
    }
}

void KNStdLibCategoryList::addCategory(const QString &title,
                                       const QPixmap &icon)
{
    CategoryItem item;
    item.title=title;
    item.icon=icon;
    if(m_categories.isEmpty())
    {
        //This is the first item.
        m_button->setCategoryIcon(icon);
        m_button->setCategoryText(title);
    }
    m_categories.append(item);
}

KNStdLibCategoryButton *KNStdLibCategoryList::listButton()
{
    return m_button;
}
