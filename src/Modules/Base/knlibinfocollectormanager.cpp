#include "knlibinfocollectormanager.h"

KNLibInfoCollectorManager::KNLibInfoCollectorManager(QObject *parent) :
    QObject(parent)
{
}

void KNLibInfoCollectorManager::addAnalysisList(const QModelIndex &index,
                                                const QString &filePath)
{
    Q_UNUSED(index);
    Q_UNUSED(filePath);
}
