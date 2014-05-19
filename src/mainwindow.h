#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

class QCloseEvent;
class KNPluginBase;
class KNStdLibCategorySwitcher;
class KNStdLibHeaderSwitcher;
class KNGlobal;
class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    explicit MainWindow(QWidget *parent = 0);

signals:

public slots:
    void addPlugin(KNPluginBase *plugin);

protected:
    void closeEvent(QCloseEvent *event);

private slots:

private:
    KNStdLibCategorySwitcher *m_mainWidget;
    KNStdLibHeaderSwitcher *m_headerWidget;
    KNGlobal *m_global;
    QString m_databaseFile;
};

#endif // MAINWINDOW_H
