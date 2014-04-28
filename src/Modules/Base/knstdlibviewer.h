#ifndef KNSTDLIBVIEWER_H
#define KNSTDLIBVIEWER_H

#include "knlibviewer.h"

class QBoxLayout;
class KNStdLibViewerCategory;
class KNWidgetSwitcher;
class KNStdLibViewer : public KNLibViewer
{
    Q_OBJECT
public:
    explicit KNStdLibViewer(QWidget *parent = 0);
    void addCategory(const QPixmap &icon,
                     const QString &category,
                     QWidget *widget);
    void setCategoryIndex(const int &index);

signals:

public slots:

protected:

private:
    QBoxLayout *m_layout;
    KNStdLibViewerCategory *m_category;
    KNWidgetSwitcher *m_listview;
};

#endif // KNSTDLIBVIEWER_H
