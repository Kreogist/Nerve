#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

class QCloseEvent;
class KNPluginBase;
class KNFontManager;
class KNStdLibCategorySwitcher;
class KNStdLibHeaderSwitcher;
class KNStdLibHeaderContainer;
class KNStdLibCategoryList;
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

private:
    KNStdLibCategorySwitcher *m_mainWidget;
    KNStdLibCategoryList *m_categoryList;
    KNStdLibHeaderContainer *m_headerContainer;
    KNStdLibHeaderSwitcher *m_headerSwitcher;
    KNGlobal *m_global;
    KNFontManager *m_fontManager;
    QString m_databaseFile;
};

#endif // MAINWINDOW_H
