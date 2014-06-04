#ifndef KNSTDLIBCATEGORYSWITCHER_H
#define KNSTDLIBCATEGORYSWITCHER_H

#include "knlibcategoryswitcher.h"

class QBoxLayout;
class KNVerticalWidgetSwitcher;
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
    KNVerticalWidgetSwitcher *m_categoryWidgets;
    QColor m_coverColor;
};

#endif // KNSTDLIBCATEGORYSWITCHER_H
