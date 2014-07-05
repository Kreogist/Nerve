#include "../../Base/knlibinfocollectormanager.h"

#include "knmusiclibrarymodelbase.h"

KNMusicLibraryModelBase::KNMusicLibraryModelBase(QObject *parent) :
    KNMusicModelBase(parent)
{
    ;
}

KNLibInfoCollectorManager *KNMusicLibraryModelBase::infoCollectorManager()
{
    return m_infoCollectorManager;
}

void KNMusicLibraryModelBase::setInfoCollectorManager(KNLibInfoCollectorManager *infoCollectorManager)
{
    m_infoCollectorManager=infoCollectorManager;
    connect(m_infoCollectorManager, &KNLibInfoCollectorManager::requireUpdateRowInfo,
            this, &KNMusicLibraryModelBase::onActionUpdateRowInfo);
}
