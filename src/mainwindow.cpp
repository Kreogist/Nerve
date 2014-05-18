#include <QFile>
#include <QDataStream>
#include <QCloseEvent>

#include "Modules/Base/knpluginbase.h"
#include "Modules/Base/knstdlibcategoryswitcher.h"
#include "Modules/Base/knstdlibheaderswitcher.h"

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

    m_headerWidget=new KNStdLibHeaderSwitcher(this);
    addDockWidget(Qt::TopDockWidgetArea, m_headerWidget);
    m_mainWidget=new KNStdLibCategorySwitcher(this);
    setCentralWidget(m_mainWidget);

    KNMusicPlugin *musicPlugin=new KNMusicPlugin(this);
    addPlugin(musicPlugin);
}

void MainWindow::addPlugin(KNPluginBase *plugin)
{
    connect(plugin, &KNPluginBase::requireAddCategory,
            m_mainWidget, &KNStdLibCategorySwitcher::addCategory);
    connect(plugin, &KNPluginBase::requireAddHeader,
            m_headerWidget, &KNStdLibHeaderSwitcher::addWidget);
    plugin->applyPlugin();
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    event->accept();
}
