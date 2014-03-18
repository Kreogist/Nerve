#ifndef KNSTDLIBCATEGORYLIST_H
#define KNSTDLIBCATEGORYLIST_H

#include "knlibcategorylist.h"

class QBoxLayout;
class KNStdLibCategoryButton : public QWidget
{
    Q_OBJECT
public:
    explicit KNStdLibCategoryButton(QWidget *parent = 0);

private:

};

class KNStdLibCategoryList : public KNLibCategoryList
{
    Q_OBJECT
public:
    explicit KNStdLibCategoryList(QWidget *parent = 0);

signals:

public slots:
    void addCategory(const QPixmap &icon, const QString &title);

private:
    QBoxLayout *m_layout;
};

#endif // KNSTDLIBCATEGORYLIST_H
