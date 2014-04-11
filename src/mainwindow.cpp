#include <QFile>
#include <QDataStream>
#include <QCloseEvent>

#include "Modules/Base/knpluginbase.h"
#include "Modules/Base/knstdlibcategoryswitcher.h"

#include "Modules/knglobal.h"

#include "Modules/Music/knmusicpluin.h"

#include "mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent)
{
    setContentsMargins(0,0,0,0);

    m_global=KNGlobal::instance();
    m_global->setMainWindow(this);

    m_mainWidget=new KNStdLibCategorySwitcher(this);
    setCentralWidget(m_mainWidget);

    KNMusicPluin *musicPlugin=new KNMusicPluin(this);
    addPlugin(musicPlugin);

    readDatabase();
}

void MainWindow::addPlugin(KNPluginBase *plugin)
{
    connect(plugin, &KNPluginBase::requireAddCategory,
            m_mainWidget, &KNStdLibCategorySwitcher::addCategory);
    connect(this, SIGNAL(requireReadData()),
            plugin, SLOT(readDatabase()));
    connect(this, SIGNAL(requireWriteData()),
            plugin, SLOT(writeDatabase()));
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
