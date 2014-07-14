#ifndef KNSTATUSBAR_H
#define KNSTATUSBAR_H

#include "../Base/knlibstatusbar.h"

class QBoxLayout;
class KNStatusBar : public KNLibStatusBar
{
    Q_OBJECT
public:
    explicit KNStatusBar(QWidget *parent = 0);

signals:

public slots:

private:
    QBoxLayout *m_mainLayout;
};

#endif // KNSTATUSBAR_H
