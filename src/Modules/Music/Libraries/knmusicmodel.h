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

public slots:
    void appendMusic(const QStringList &info, const QPixmap &coverImage);
    void retranslate();
    void retranslateAndSet();

private:
    QString m_header[KNMusicGlobal::MusicDataCount];
};

#endif // KNMUSICMODEL_H
