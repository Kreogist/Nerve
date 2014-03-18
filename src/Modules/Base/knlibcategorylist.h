#ifndef KNLIBCATEGORYLIST_H
#define KNLIBCATEGORYLIST_H

#include <QWidget>

class KNLibCategoryList : public QWidget
{
    Q_OBJECT
public:
    explicit KNLibCategoryList(QWidget *parent = 0);

signals:

public slots:
    virtual void addCategory(const QPixmap &icon,
                             const QString &title);

};

#endif // KNLIBCATEGORYLIST_H
