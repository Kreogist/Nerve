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
    void setCategoryIndex(const int &index);
    int categoryIndex() const;
    void moveLeft();
    void moveRight();

signals:

public slots:
    void addCategory(const QPixmap &icon,
                     const QString &category,
                     QWidget *widget);
    void setContentsFocus();

protected:

private:
    QBoxLayout *m_layout;
    KNStdLibViewerCategory *m_category;
    KNWidgetSwitcher *m_widgetSwicher;
};

#endif // KNSTDLIBVIEWER_H
