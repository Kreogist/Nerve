#ifndef KNLIBHEADERSWITCHER_H
#define KNLIBHEADERSWITCHER_H

#include <QDockWidget>

#include <QList>
#include <QWidget>

class QResizeEvent;
class KNLibHeaderSwitcher : public QWidget
{
    Q_OBJECT
public:
    explicit KNLibHeaderSwitcher(QWidget *parent = 0);
    virtual void addWidget(QWidget *widget);
    virtual void setCurrentIndex(const int &index);
    int currentIndex() const;
    int widgetCount() const;
    QWidget *widgetAt(const int &index);
    QWidget *currentWidget() const;

protected:
    void resizeEvent(QResizeEvent *event);

private:
    QList<QWidget *> m_stackedWidgets;
    int m_currentIndex=0;
};

#endif // KNLIBHEADERSWITCHER_H
