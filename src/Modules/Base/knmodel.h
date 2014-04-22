#ifndef KNMODEL_H
#define KNMODEL_H

#include <QStandardItemModel>

class KNModel : public QStandardItemModel
{
    Q_OBJECT
public:
    explicit KNModel(QObject *parent = 0);
    void resetHeaderOrder();
    virtual bool readFromDataStream(QDataStream &stream);
    virtual bool writeToDataStream(QDataStream &stream);

signals:
    void requireResetHeaderOrder();

public slots:
    virtual void retranslate();
    virtual void retranslateAndSet();

};

#endif // KNMODEL_H
