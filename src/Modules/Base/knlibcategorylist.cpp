#include "knlibcategorylist.h"

KNLibCategoryList::KNLibCategoryList(QWidget *parent) :
    QWidget(parent)
{
}

int KNLibCategoryList::currentIndex() const
{
    return m_currentIndex;
}

void KNLibCategoryList::addCategory(const QString &title,
                                    const QPixmap &icon)
{
    Q_UNUSED(title);
    Q_UNUSED(icon);
}

void KNLibCategoryList::setCurrentIndex(const int &index)
{
    Q_UNUSED(index);
}
