#include "Widgets/knstoreviewer.h"

#include "knstoreplugin.h"

KNStorePlugin::KNStorePlugin(QObject *parent) :
    KNPluginBase(parent)
{
    m_storeViewer=new KNStoreViewer;
    test=new QWidget;
}

void KNStorePlugin::applyPlugin()
{
    emit requireAddCategory("Store",
                            QPixmap(":/Store/Resources/Store/icon.png"),
                            m_storeViewer);
    emit requireAddHeader(test);
}
