#include <QBoxLayout>
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

    QWidget *holder=new QWidget(this);
    holder->setContentsMargins(0,0,0,0);
    setCentralWidget(holder);

    QBoxLayout *mainLayout=new QBoxLayout(QBoxLayout::TopToBottom, holder);
    mainLayout->setContentsMargins(0,0,0,0);
    mainLayout->setSpacing(0);
    holder->setLayout(mainLayout);

    m_headerWidget=new KNStdLibHeaderSwitcher(this);
    mainLayout->addWidget(m_headerWidget);
    m_mainWidget=new KNStdLibCategorySwitcher(this);
    mainLayout->addWidget(m_mainWidget, 1);

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
