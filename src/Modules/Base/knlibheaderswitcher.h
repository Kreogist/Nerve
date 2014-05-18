#ifndef KNLIBHEADERSWITCHER_H
#define KNLIBHEADERSWITCHER_H

#include <QDockWidget>

#include <QList>
#include <QWidget>

class QResizeEvent;
class KNLibHeaderSwitcherWidget : public QWidget
{
    Q_OBJECT
public:
    explicit KNLibHeaderSwitcherWidget(QWidget *parent = 0);
    virtual void addWidget(QWidget *widget);
    virtual void setCurrentIndex(const int &index);
    int currentIndex() const;
    int widgetCount() const;
    QWidget *widgetAt(const int &index);

protected:
    void resizeEvent(QResizeEvent *event);

private:
    QList<QWidget *> m_stackedWidgets;
    int m_currentIndex=0;
};

class KNLibHeaderSwitcher : public QDockWidget
{
    Q_OBJECT
public:
    explicit KNLibHeaderSwitcher(QWidget *parent = 0);
    void addWidget(QWidget *widget);
    void setCurrentIndex(const int &index);
    int currentIndex() const;
    int widgetCount() const;
    void setSwitcher(KNLibHeaderSwitcherWidget *switcher);

signals:

public slots:

private:
    KNLibHeaderSwitcherWidget *m_headerSwitcherWidget;
};

#endif // KNLIBHEADERSWITCHER_H
