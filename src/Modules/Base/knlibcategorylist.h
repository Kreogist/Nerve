#ifndef KNLIBCATEGORYLIST_H
#define KNLIBCATEGORYLIST_H

#include <QWidget>

class KNLibCategoryList : public QWidget
{
    Q_OBJECT
public:
    explicit KNLibCategoryList(QWidget *parent = 0);
    int currentIndex() const;

signals:
    void categoryIndexChanged(const int &index);

public slots:
    virtual void addCategory(const QPixmap &icon,
                             const QString &title);
    virtual void setCurrentIndex(const int &index);

private:
    int m_currentIndex=-1;
};

#endif // KNLIBCATEGORYLIST_H
