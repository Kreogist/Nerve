#ifndef KNMUSICMODEL_H
#define KNMUSICMODEL_H

#include <QStringList>

#include "../knmusicglobal.h"

#include "../../Base/knmodel.h"

class KNMusicModel : public KNModel
{
    Q_OBJECT
public:
    explicit KNMusicModel(QObject *parent = 0);
    bool readFromDataStream(QDataStream &stream);
    bool writeToDataStream(QDataStream &stream);
    QString filePathFromIndex(const QModelIndex &index);

signals:
    void musicAppend(const QModelIndex &index);

public slots:
    void appendMusic(const QStringList &info,
                     const KNMusicGlobal::MusicDetailsInfo &datas);
    void retranslate();
    void retranslateAndSet();

private:
    void updateMusicInfo(const QModelIndex &index);
    KNMusicGlobal *musicGlobal;
};

#endif // KNMUSICMODEL_H
