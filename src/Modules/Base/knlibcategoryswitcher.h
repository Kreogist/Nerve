#ifndef KNLIBCATEGORYSWITCHER_H
#define KNLIBCATEGORYSWITCHER_H

#include <QWidget>

class KNLibCategorySwitcher : public QWidget
{
    Q_OBJECT
public:
    explicit KNLibCategorySwitcher(QWidget *parent = 0);

signals:

public slots:
    virtual void addCategory(const QString &name,
                             const QPixmap &icon,
                             QWidget *centralWidget)=0;
    virtual void setCentralWidgetIndex(const int &index)=0;

};

#endif // KNLIBCATEGORYSWITCHER_H
