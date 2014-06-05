#ifndef KNSTDLIBCATEGORYLIST_H
#define KNSTDLIBCATEGORYLIST_H

#include <QPixmap>
#include <QList>

#include "knlibcategorylist.h"

class QBoxLayout;
class QSignalMapper;
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
    void requireSwitchTo(const int &index);

public slots:
    void addCategory(const QString &title, const QPixmap &icon);

private slots:
    void showCategorySwitcher();
    void hideCategorySwitcher();
    void onActionSwitch(const int &index);

private:
    struct CategoryItem
    {
        QPixmap icon;
        QString title;
    };
    QList<CategoryItem> m_categories;
    QBoxLayout *m_layout, *m_buttonLayout;
    QTimeLine *m_expandCategory, *m_foldCategory;
    QSignalMapper *m_selectMapper;
    int m_switchToIndex=-1, m_currentIndex=-1;
    KNStdLibCategoryButton *m_button;
};

#endif // KNSTDLIBCATEGORYLIST_H
