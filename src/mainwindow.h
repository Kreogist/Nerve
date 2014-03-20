#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

/*#include <QList>
#include <QThread>*/

class KNPluginBase;
class KNStdLibCategorySwitcher;
class KNGlobal;
class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    explicit MainWindow(QWidget *parent = 0);

signals:

public slots:
    void addPlugin(KNPluginBase *plugin);

private slots:

private:
    KNStdLibCategorySwitcher *m_mainWidget;
    KNGlobal *m_global;
    //QList<QThread> m_pluginThread;
};

#endif // MAINWINDOW_H
