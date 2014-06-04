#include <QBoxLayout>
#include <QTimeLine>
#include <QPropertyAnimation>

#include <QDebug>

#include "knstdlibcategorybutton.h"

#include "knstdlibcategorylist.h"

KNStdLibCategoryList::KNStdLibCategoryList(QWidget *parent) :
    KNLibCategoryList(parent)
{
    setAutoFillBackground(true);
    setContentsMargins(0,0,0,0);
    setFixedHeight(0);
    QPalette pal=palette();
    pal.setColor(QPalette::Window, QColor(0x29, 0x2a, 0x2e));
    setPalette(pal);
    m_button=new KNStdLibCategoryButton;
    connect(m_button, &KNStdLibCategoryButton::requireShowCategorySelect,
            this, &KNStdLibCategoryList::showCategory);
    connect(m_button, &KNStdLibCategoryButton::requireHideCategorySelect,
            this, &KNStdLibCategoryList::hideCategory);

    m_layout=new QBoxLayout(QBoxLayout::LeftToRight, this);
    m_layout->setContentsMargins(0,0,0,0);
    m_layout->setSpacing(0);
    setLayout(m_layout);

    m_expandCategory=new QTimeLine(200, this);
    m_expandCategory->setUpdateInterval(10);
    m_expandCategory->setEndFrame(100);
    connect(m_expandCategory, &QTimeLine::frameChanged,
            this, &KNStdLibCategoryList::setFixedHeight);
    connect(m_expandCategory, &QTimeLine::valueChanged,
            this, &KNStdLibCategoryList::requireChangeOpacity);

    m_foldCategory=new QTimeLine(200, this);
    m_foldCategory->setUpdateInterval(10);
    m_foldCategory->setEndFrame(0);
    connect(m_foldCategory, &QTimeLine::frameChanged,
            this, &KNStdLibCategoryList::setFixedHeight);
    connect(m_foldCategory, &QTimeLine::finished,
            this, &KNStdLibCategoryList::requireEnableContent);
    connect(m_foldCategory, &QTimeLine::valueChanged,
            [=](const qreal &value)
            {
                emit requireChangeOpacity(1.0-value);
            });
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
    //Add to list.
    m_categories.append(item);
}

void KNStdLibCategoryList::showCategory()
{
    emit requireDisableContent();
    m_expandCategory->setStartFrame(height());
    m_expandCategory->start();
}

void KNStdLibCategoryList::hideCategory()
{
    m_foldCategory->setStartFrame(height());
    m_foldCategory->start();
}

KNStdLibCategoryButton *KNStdLibCategoryList::listButton()
{
    return m_button;
}
