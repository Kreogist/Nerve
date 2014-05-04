#ifndef KNLIBINFOCOLLECTORMANAGER_H
#define KNLIBINFOCOLLECTORMANAGER_H

#include <QModelIndex>
#include <QObject>

class KNLibInfoCollectorManager : public QObject
{
    Q_OBJECT
public:
    explicit KNLibInfoCollectorManager(QObject *parent = 0);

signals:
    void requireUpdateRowInfo(const QModelIndex &index);
    void requireSortData();

public slots:
    virtual void addAnalysisList(const QModelIndex &index,
                                 const QString &filePath);

};

#endif // KNLIBINFOCOLLECTORMANAGER_H
