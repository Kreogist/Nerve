#ifndef KNMENU_H
#define KNMENU_H

#include <QMenu>

class QShowEvent;
class QPropertyAnimation;
class KNMenu : public QMenu
{
    Q_OBJECT
public:
    explicit KNMenu(QWidget *parent = 0);

signals:

public slots:

protected:
    void showEvent(QShowEvent *e);

private:
    QPropertyAnimation *m_showAnimation;
};

#endif // KNMENU_H
