#ifndef KNMUSICMODEL_H
#define KNMUSICMODEL_H

#include <QStringList>

#include "../knmusicglobal.h"

#include "../../Base/knmodel.h"

class KNMusicInfoCollectorManager;
class KNMusicModel : public KNModel
{
    Q_OBJECT
public:
    explicit KNMusicModel(QObject *parent = 0);
    bool readFromDataStream(QDataStream &stream);
    bool writeToDataStream(QDataStream &stream);
    QString filePathFromIndex(const QModelIndex &index);
    void addRawFileItem(const QString &filePath);
    void updateIndexInfo(const QModelIndex &index,
                         const QString &filePath);
    void setInfoCollectorManager(KNLibInfoCollectorManager *infoCollectorManager);

signals:
    void musicAppend(const QModelIndex &index);
    void musicDataUpdate(const QModelIndex &index);

public slots:
    void retranslate();
    void retranslateAndSet();

protected slots:
    void onActionUpdateRowInfo(const QModelIndex &index);

private:
    KNMusicGlobal *m_musicGlobal;
    KNMusicInfoCollectorManager *m_infoCollectorManager;
};

#endif // KNMUSICMODEL_H
