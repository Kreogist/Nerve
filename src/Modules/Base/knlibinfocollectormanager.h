#ifndef KNLIBINFOCOLLECTORMANAGER_H
#define KNLIBINFOCOLLECTORMANAGER_H

#include <QModelIndex>
#include <QObject>

class QStandardItem;
class KNLibInfoCollectorManager : public QObject
{
    Q_OBJECT
public:
    explicit KNLibInfoCollectorManager(QObject *parent = 0);

signals:
    void requireUpdateRowInfo();
    void removedItemClear();

public slots:
    virtual void addAnalysisList(QStandardItem *index,
                                 QString filePath);

};

#endif // KNLIBINFOCOLLECTORMANAGER_H
