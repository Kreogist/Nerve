#ifndef KNMUSICMODEL_H
#define KNMUSICMODEL_H

#include <QStringList>

#include "../knmusicglobal.h"

#include <QStandardItemModel>

class KNMusicModel : public QStandardItemModel
{
    Q_OBJECT
public:
    explicit KNMusicModel(QObject *parent = 0);
    void resetHeader();

signals:
    void requireResetHeader();
    void musicAppend(const QModelIndex &index);

public slots:
    void appendMusic(const QStringList &info,
                     const KNMusicGlobal::MusicDetailsInfo &datas);
    void retranslate();
    void retranslateAndSet();

private:
    KNMusicGlobal *instance;
};

#endif // KNMUSICMODEL_H
