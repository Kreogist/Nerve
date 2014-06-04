#ifndef KNLIBCATEGORYSWITCHER_H
#define KNLIBCATEGORYSWITCHER_H

#include <QWidget>

class KNLibCategorySwitcher : public QWidget
{
    Q_OBJECT
public:
    explicit KNLibCategorySwitcher(QWidget *parent = 0);

signals:
    void requireAddCategory(const QString &name,
                            const QPixmap &icon);

public slots:
    virtual void setCentralWidgetIndex(const int &index)=0;

};

#endif // KNLIBCATEGORYSWITCHER_H
