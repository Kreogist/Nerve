#include <QFile>
#include <QDataStream>
#include <QCloseEvent>

#include "Modules/Base/knpluginbase.h"
#include "Modules/Base/knstdlibcategoryswitcher.h"

#include "Modules/knglobal.h"

#include "Modules/Music/knmusicplugin.h"

#include "mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent)
{
    setContentsMargins(0,0,0,0);
    setMinimumSize(600, 400);

    m_global=KNGlobal::instance();
    m_global->setMainWindow(this);

    m_mainWidget=new KNStdLibCategorySwitcher(this);
    setCentralWidget(m_mainWidget);

    KNMusicPlugin *musicPlugin=new KNMusicPlugin(this);
    addPlugin(musicPlugin);

    readDatabase();
}

void MainWindow::addPlugin(KNPluginBase *plugin)
{
    connect(plugin, &KNPluginBase::requireAddCategory,
            m_mainWidget, &KNStdLibCategorySwitcher::addCategory);
    connect(this, &MainWindow::requireReadData,
            plugin, &KNPluginBase::readDatabase);
    connect(this, &MainWindow::requireWriteData,
            plugin, &KNPluginBase::writeDatabase);
    plugin->applyPlugin();
}

void MainWindow::readDatabase()
{
    emit requireReadData();
}

void MainWindow::writeDatabase()
{
    emit requireWriteData();
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    writeDatabase();
    event->accept();
}
