#ifndef KNSTDLIBVIEWER_H
#define KNSTDLIBVIEWER_H

#include "knlibviewer.h"

class QBoxLayout;
class KNLibSearcher;
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
    void setSearcher(KNLibSearcher *searcher);
    void moveLeft();
    void moveRight();

signals:

public slots:

protected:
    KNLibSearcher *m_searcher;

private:
    QBoxLayout *m_layout;
    KNStdLibViewerCategory *m_category;
    KNWidgetSwitcher *m_listview;
};

#endif // KNSTDLIBVIEWER_H
