#include "../knlocale.h"

#include "knmodel.h"

KNModel::KNModel(QObject *parent) :
    QStandardItemModel(parent)
{
    retranslate();

    connect(KNLocale::instance(), SIGNAL(languageChanged()),
            this, SLOT(retranslateAndSet()));
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
