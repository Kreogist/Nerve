#include <QBoxLayout>
#include <QPushButton>

#include "knstdlibcategorylist.h"

KNStdLibCategoryButton::KNStdLibCategoryButton(QWidget *parent) :
    QWidget(parent)
{
    ;
}

KNStdLibCategoryList::KNStdLibCategoryList(QWidget *parent) :
    KNLibCategoryList(parent)
{
    setContentsMargins(0,0,0,0);

    m_layout=new QBoxLayout(QBoxLayout::LeftToRight, this);
    m_layout->setContentsMargins(0,0,0,0);
    m_layout->setSpacing(0);
    setLayout(m_layout);
}

void KNStdLibCategoryList::addCategory(const QPixmap &icon,
                                       const QString &title)
{
    QPushButton *button=new QPushButton(this);
    button->setText(title);
    m_layout->addWidget(button);
}
