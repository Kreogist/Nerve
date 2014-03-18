#ifndef KNGLOBAL_H
#define KNGLOBAL_H

#include <QWidget>
#include <QObject>

class KNGlobal : public QObject
{
    Q_OBJECT
public:
    static KNGlobal *instance();

    QWidget *mainWindow() const;
    void setMainWindow(QWidget *mainWindow);

signals:

public slots:

private:
    KNGlobal();
    static KNGlobal *m_instance;
    QWidget *m_mainWindow;
};

#endif // KNGLOBAL_H
