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

void KNModel::setDatabase(KNStdLibDatabase *database)
{
    ;
}


void KNModel::addRawFileItem(QString filePath)
{
    Q_UNUSED(filePath);
}

void KNModel::addRawFileItems(QStringList fileList)
{
    Q_UNUSED(fileList);
}

