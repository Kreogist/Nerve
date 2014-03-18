#ifndef KNSTDLIBCATEGORYSWITCHER_H
#define KNSTDLIBCATEGORYSWITCHER_H

#include "knlibcategoryswitcher.h"

class KNWidgetSwitcher;
class QBoxLayout;
class KNStdLibCategorySwitcher : public KNLibCategorySwitcher
{
    Q_OBJECT
public:
    explicit KNStdLibCategorySwitcher(QWidget *parent = 0);

signals:

public slots:
    void addCategory(const QString &name,
                     const QPixmap &icon,
                     QWidget *centralWidget);
    void setCentralWidgetIndex(const int &index);

private:
    QBoxLayout *m_layout;
    KNWidgetSwitcher *m_categoryWidgets;
};

#endif // KNSTDLIBCATEGORYSWITCHER_H
