#ifndef KNMODEL_H
#define KNMODEL_H

#include <QStringList>

#include <QStandardItemModel>

class KNLibInfoCollectorManager;
class KNModel : public QStandardItemModel
{
    Q_OBJECT
public:
    explicit KNModel(QObject *parent = 0);
    void resetHeaderOrder();
    virtual void addRawFileItem(QString filePath);
    virtual void addRawFileItems(QStringList fileList);
    virtual bool readFromDataStream(QDataStream &stream);
    virtual bool writeToDataStream(QDataStream &stream);
    KNLibInfoCollectorManager *infoCollectorManager() const;
    virtual void setInfoCollectorManager(KNLibInfoCollectorManager *infoCollectorManager);

signals:
    void requireResetHeaderOrder();
    void requireResort();
    void requireUpdateNextRow();

public slots:
    virtual void retranslate();
    virtual void retranslateAndSet();

protected slots:
    virtual void onActionUpdateRowInfo();

private:
    KNLibInfoCollectorManager *m_infoCollectorManager;
};

#endif // KNMODEL_H
