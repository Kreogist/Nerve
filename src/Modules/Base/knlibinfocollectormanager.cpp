#include "knlibinfocollectormanager.h"

KNLibInfoCollectorManager::KNLibInfoCollectorManager(QObject *parent) :
    QObject(parent)
{
}

void KNLibInfoCollectorManager::addAnalysisList(int index,
                                                QString filePath)
{
    Q_UNUSED(index);
    Q_UNUSED(filePath);
}
