#ifndef KNSTDLIBHEADERSWITCHER_H
#define KNSTDLIBHEADERSWITCHER_H

#include "knlibheaderswitcher.h"

class QPropertyAnimation;
class QParallelAnimationGroup;
class QResizeEvent;
class KNStdLibHeaderSwitcher : public KNLibHeaderSwitcher
{
    Q_OBJECT
public:
    explicit KNStdLibHeaderSwitcher(QWidget *parent = 0);
    void setCurrentIndex(const int &index);

private slots:
    void onActionAnimationComplete();

protected:
    void resizeEvent(QResizeEvent *event);

private:
    int m_aboutToBeCurrentIndex;
    QPropertyAnimation *m_moveOut, *m_moveIn;
    QParallelAnimationGroup *m_movedGroup;
};

#endif // KNSTDLIBHEADERSWITCHER_H
