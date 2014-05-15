#include "knstdlibdatabase.h"

KNStdLibDatabase::KNStdLibDatabase(QObject *parent) :
    KNLibDatabase(parent)
{
}

void KNStdLibDatabase::setModel(QAbstractItemModel *model)
{
    Q_UNUSED(model);
}

void KNStdLibDatabase::load()
{
    KNLibDatabase::load();
    recoverData();
}

void KNStdLibDatabase::recoverData()
{
    ;
}
