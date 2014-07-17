#ifndef KNSTDLIBDATABASE_H
#define KNSTDLIBDATABASE_H

#include <QModelIndex>

#include "knlibdatabase.h"

class QAbstractItemModel;
class KNStdLibDatabase : public KNLibDatabase
{
    Q_OBJECT
public:
    explicit KNStdLibDatabase(QObject *parent = 0);
    virtual void setModel(QAbstractItemModel *model);
    void load();
    virtual void recoverData();

signals:
    void recoverComplete();

public slots:

private:
};

#endif // KNSTDLIBDATABASE_H
