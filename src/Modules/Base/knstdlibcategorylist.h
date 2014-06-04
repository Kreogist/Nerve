#ifndef KNSTDLIBCATEGORYLIST_H
#define KNSTDLIBCATEGORYLIST_H

#include <QPixmap>
#include <QList>

#include "knlibcategorylist.h"

class QBoxLayout;
class KNStdLibCategoryButton;
class KNStdLibCategoryList : public KNLibCategoryList
{
    Q_OBJECT
public:
    explicit KNStdLibCategoryList(QWidget *parent = 0);
    ~KNStdLibCategoryList();
    KNStdLibCategoryButton *listButton();

signals:

public slots:
    void addCategory(const QString &title, const QPixmap &icon);

private:
    struct CategoryItem
    {
        QPixmap icon;
        QString title;
    };
    QList<CategoryItem> m_categories;
    QBoxLayout *m_layout;
    KNStdLibCategoryButton *m_button;
};

#endif // KNSTDLIBCATEGORYLIST_H
