#ifndef KNLIBDIRMODEL_H
#define KNLIBDIRMODEL_H

#include <QDir>
#include <QFileInfo>
#include <QList>
#include <QStandardItemModel>

class KNLibDirModel : public QStandardItemModel
{
    Q_OBJECT
public:
    explicit KNLibDirModel(QObject *parent = 0);
    virtual void refresh();

signals:

public slots:

};

#endif // KNLIBDIRMODEL_H
