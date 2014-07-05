#ifndef KNMODEL_H
#define KNMODEL_H

#include <QStringList>

#include <QStandardItemModel>

class KNModel : public QStandardItemModel
{
    Q_OBJECT
public:
    explicit KNModel(QObject *parent = 0);
    void resetHeaderOrder();
    virtual void addRawFileItem(QString filePath);
    virtual void addRawFileItems(QStringList fileList);

signals:
    void requireResetHeaderOrder();
    void requireResort();
    void requireUpdateNextRow();

public slots:
    virtual void retranslate();
    virtual void retranslateAndSet();

protected slots:
    virtual void onActionUpdateRowInfo();
};

#endif // KNMODEL_H
