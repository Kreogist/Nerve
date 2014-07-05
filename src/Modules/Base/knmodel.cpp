#include "../knlocale.h"

#include "knmodel.h"

KNModel::KNModel(QObject *parent) :
    QStandardItemModel(parent)
{
    retranslate();

    connect(KNLocale::instance(), &KNLocale::languageChanged,
            this, &KNModel::retranslateAndSet);
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

void KNModel::addRawFileItem(QString filePath)
{
    Q_UNUSED(filePath);
}

void KNModel::addRawFileItems(QStringList fileList)
{
    Q_UNUSED(fileList);
}

