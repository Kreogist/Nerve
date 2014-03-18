#ifndef KNMUSICMODEL_H
#define KNMUSICMODEL_H

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

public slots:
    void retranslate();
    void retranslateAndSet();

private:
    QString m_header[KNMusicGlobal::MusicHeaderCount];
};

#endif // KNMUSICMODEL_H
