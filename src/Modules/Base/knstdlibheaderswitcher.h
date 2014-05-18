#ifndef KNSTDLIBHEADERSWITCHER_H
#define KNSTDLIBHEADERSWITCHER_H

#include "knlibheaderswitcher.h"

class QPropertyAnimation;
class QParallelAnimationGroup;
class KNStdLibHeaderSwitcherWidget : public KNLibHeaderSwitcherWidget
{
    Q_OBJECT
public:
    explicit KNStdLibHeaderSwitcherWidget(QWidget *parent = 0);
    void setCurrentIndex(const int &index);

private slots:
    void onActionAnimationComplete();

private:
    int m_aboutToBeCurrentIndex;
    QPropertyAnimation *m_moveOut, *m_moveIn;
    QParallelAnimationGroup *m_movedGroup;
};

class KNStdLibHeaderSwitcher : public KNLibHeaderSwitcher
{
    Q_OBJECT
public:
    explicit KNStdLibHeaderSwitcher(QWidget *parent = 0);

signals:

public slots:

private:
    KNStdLibHeaderSwitcherWidget *m_swicher;
};

#endif // KNSTDLIBHEADERSWITCHER_H