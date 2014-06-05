#ifndef KNSIDESHADOWS_H
#define KNSIDESHADOWS_H

#include <QWidget>

class QPaintEvent;
class KNTopSideShadow : public QWidget
{
    Q_OBJECT
public:
    explicit KNTopSideShadow(QWidget *parent = 0);

protected:
    void paintEvent(QPaintEvent *event);
};

class KNBottomSideShadow : public QWidget
{
    Q_OBJECT
public:
    explicit KNBottomSideShadow(QWidget *parent = 0);

protected:
    void paintEvent(QPaintEvent *event);
};

class KNRightSideShadow : public QWidget
{
    Q_OBJECT
public:
    explicit KNRightSideShadow(QWidget *parent = 0);

protected:
    void paintEvent(QPaintEvent *event);
};

class KNLeftSideShadow : public QWidget
{
    Q_OBJECT
public:
    explicit KNLeftSideShadow(QWidget *parent = 0);

protected:
    void paintEvent(QPaintEvent *event);
};

#endif // KNSIDESHADOWS_H
