#include "knlibinfocollectormanager.h"

#include "../knlocale.h"

#include "knmodel.h"

KNModel::KNModel(QObject *parent) :
    QStandardItemModel(parent)
{
    retranslate();

    connect(KNLocale::instance(), SIGNAL(languageChanged()),
            this, SLOT(retranslateAndSet()));
    connect(this, &KNModel::requireUpdateNextRow,
            this, &KNModel::onActionUpdateRowInfo);
}

void KNModel::resetHeaderOrder()
{
    emit requireResetHeaderOrder();
}

bool KNModel::readFromDataStream(QDataStream &stream)
{
    Q_UNUSED(stream);
}

bool KNModel::writeToDataStream(QDataStream &stream)
{
    Q_UNUSED(stream);
}

void KNModel::retranslate()
{
    ;
}

void KNModel::retranslateAndSet()
{
    retranslate();
}

void KNModel::onActionUpdateRowInfo()
{
    ;
}

KNLibInfoCollectorManager *KNModel::infoCollectorManager() const
{
    return m_infoCollectorManager;
}

void KNModel::setInfoCollectorManager(KNLibInfoCollectorManager *infoCollectorManager)
{
    m_infoCollectorManager=infoCollectorManager;
    connect(m_infoCollectorManager, &KNLibInfoCollectorManager::requireUpdateRowInfo,
            this, &KNModel::onActionUpdateRowInfo);
}


void KNModel::addRawFileItem(QString filePath)
{
    Q_UNUSED(filePath);
}

