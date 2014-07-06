#include <QBoxLayout>
#include <QCloseEvent>

#include "Modules/Base/knpluginbase.h"
#include "Modules/Base/knstdlibheadercontainer.h"
#include "Modules/Base/knstdlibcategoryswitcher.h"
#include "Modules/Base/knstdlibheaderswitcher.h"
#include "Modules/Base/knstdlibcategorylist.h"

#include "Modules/knglobal.h"
#include "Modules/knfontmanager.h"

#include "Modules/Music/knmusicplugin.h"
#include "Modules/Store/knstoreplugin.h"

#include "mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent)
{
    setContentsMargins(0,0,0,0);
    setMinimumSize(730, 432);

    m_global=KNGlobal::instance();
    m_global->setMainWindow(this);

    m_fontManager=KNFontManager::instance();
    m_fontManager->loadCustomFont();
    m_fontManager->setGlobalFont("WenQuanYi Micro Hei");

    QWidget *holder=new QWidget(this);
    holder->setContentsMargins(0,0,0,0);
    setCentralWidget(holder);

    QBoxLayout *mainLayout=new QBoxLayout(QBoxLayout::TopToBottom, holder);
    mainLayout->setContentsMargins(0,0,0,0);
    mainLayout->setSpacing(0);
    holder->setLayout(mainLayout);

    m_headerContainer=new KNStdLibHeaderContainer(this);
    m_categoryList=new KNStdLibCategoryList(this);
    m_headerSwitcher=new KNStdLibHeaderSwitcher;
    m_headerContainer->addCategorySwitcher(m_categoryList->listButton());
    m_headerContainer->addHeaderSwitcher(m_headerSwitcher);
    mainLayout->addWidget(m_headerContainer);
    m_mainWidget=new KNStdLibCategorySwitcher(this);
    connect(m_mainWidget, &KNStdLibCategorySwitcher::requireAddCategory,
            m_categoryList, &KNStdLibCategoryList::addCategory);
    connect(m_categoryList, &KNStdLibCategoryList::requireDisableContent,
            m_mainWidget, &KNStdLibCategorySwitcher::disableContent);
    connect(m_categoryList, &KNStdLibCategoryList::requireEnableContent,
            m_mainWidget, &KNStdLibCategorySwitcher::enableContent);
    connect(m_categoryList, &KNStdLibCategoryList::requireChangeOpacity,
            m_mainWidget, &KNStdLibCategorySwitcher::changeOpacity);
    connect(m_categoryList, &KNStdLibCategoryList::requireSwitchTo,
            m_mainWidget, &KNStdLibCategorySwitcher::setCentralWidgetIndex);
    connect(m_categoryList, &KNStdLibCategoryList::requireSwitchTo,
            m_headerSwitcher, &KNStdLibHeaderSwitcher::setCurrentIndex);
    mainLayout->addWidget(m_categoryList);
    mainLayout->addWidget(m_mainWidget, 1);

    KNMusicPlugin *musicPlugin=new KNMusicPlugin(this);
    addPlugin(musicPlugin);
//    KNStorePlugin *storePlugin=new KNStorePlugin(this);
//    addPlugin(storePlugin);
}

void MainWindow::addPlugin(KNPluginBase *plugin)
{
    connect(plugin, &KNPluginBase::requireAddCategory,
            m_mainWidget, &KNStdLibCategorySwitcher::addCategory);
    connect(plugin, &KNPluginBase::requireAddHeader,
            m_headerSwitcher, &KNStdLibHeaderSwitcher::addWidget);
    plugin->applyPlugin();
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    event->accept();
}
