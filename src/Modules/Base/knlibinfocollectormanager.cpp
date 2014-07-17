#include "knlibinfocollectormanager.h"

KNLibInfoCollectorManager::KNLibInfoCollectorManager(QObject *parent) :
    QObject(parent)
{
}

void KNLibInfoCollectorManager::addAnalysisList(QStandardItem *index,
                                                QString filePath)
{
    Q_UNUSED(index);
    Q_UNUSED(filePath);
}
