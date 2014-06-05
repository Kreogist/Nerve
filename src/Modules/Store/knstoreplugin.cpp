#include "knstoreplugin.h"

KNStorePlugin::KNStorePlugin(QObject *parent) :
    KNPluginBase(parent)
{
    text=new QWidget;
    test=new QWidget;
}

void KNStorePlugin::applyPlugin()
{
    emit requireAddCategory("Store",
                            QPixmap(":/Store/Resources/Store/icon.png"),
                            text);
    emit requireAddHeader(test);
}
