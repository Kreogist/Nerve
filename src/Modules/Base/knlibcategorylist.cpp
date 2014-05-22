#include "knlibcategorylist.h"

KNLibCategoryList::KNLibCategoryList(QWidget *parent) :
    QWidget(parent)
{
}

int KNLibCategoryList::currentIndex() const
{
    return m_currentIndex;
}

void KNLibCategoryList::addCategory(const QPixmap &icon,
                                    const QString &title)
{
    ;
}

void KNLibCategoryList::setCurrentIndex(const int &index)
{
    ;
}
