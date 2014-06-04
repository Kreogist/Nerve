#ifndef KNLIBHEADERCONTAINER_H
#define KNLIBHEADERCONTAINER_H

#include <QWidget>

class KNLibHeaderSwitcher;
class KNLibHeaderContainer : public QWidget
{
    Q_OBJECT
public:
    explicit KNLibHeaderContainer(QWidget *parent = 0);
    virtual void addHeaderSwitcher(KNLibHeaderSwitcher *switcher);

signals:

public slots:

};

#endif // KNLIBHEADERCONTAINER_H
