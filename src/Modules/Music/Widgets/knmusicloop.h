#ifndef KNMUSICLOOP_H
#define KNMUSICLOOP_H

#include <QPixmap>
#include <QWidget>
#include <QLabel>

#include "../knmusicglobal.h"

class QBoxLayout;
class KNMusicLoopButton : public QLabel
{
    Q_OBJECT
public:
    explicit KNMusicLoopButton(QWidget *parent = 0);

signals:
    void clicked();

protected:
    void mousePressEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);

private:
    bool m_isPressed=false;
};

class KNMusicLoop : public QWidget
{
    Q_OBJECT
public:
    explicit KNMusicLoop(QWidget *parent = 0);

signals:
    void requireChangeLoopType(int loopType);

public slots:

private slots:
    void onActionChangeType();

private:
    QBoxLayout *m_mainLayout;
    QPixmap m_loopTypeImage[KNMusicGlobal::LoopCount];
    KNMusicLoopButton *m_loopType;
    int m_loopMode=0;
};

#endif // KNMUSICLOOP_H
