#ifndef KNSTDLIBCATEGORYLIST_H
#define KNSTDLIBCATEGORYLIST_H

#include <QPixmap>
#include <QList>

#include "knlibcategorylist.h"

class QBoxLayout;
class QTimeLine;
class KNStdLibCategoryButton;
class KNStdLibCategoryList : public KNLibCategoryList
{
    Q_OBJECT
public:
    explicit KNStdLibCategoryList(QWidget *parent = 0);
    KNStdLibCategoryButton *listButton();

signals:
    void requireDisableContent();
    void requireEnableContent();
    void requireChangeOpacity(const qreal &opacity);

public slots:
    void addCategory(const QString &title, const QPixmap &icon);

private slots:
    void showCategory();
    void hideCategory();

private:
    struct CategoryItem
    {
        QPixmap icon;
        QString title;
    };
    QList<CategoryItem> m_categories;
    QBoxLayout *m_layout;
    QTimeLine *m_expandCategory, *m_foldCategory;
    KNStdLibCategoryButton *m_button;
};

#endif // KNSTDLIBCATEGORYLIST_H
