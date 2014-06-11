#ifndef KNMUSICONLINENETEASE_H
#define KNMUSICONLINENETEASE_H

#include <QStandardItemModel>
#include "knmusiconlinebase.h"

class KNMusicOnlineNetease : public KNMusicOnlineBase
{
    Q_OBJECT
public:
    explicit KNMusicOnlineNetease(QObject *parent = 0);
    QStandardItemModel *model() const;
signals:

public slots:
    void searchMusic(const QString &title);

private slots:
    void handleReplyData(QNetworkReply *replyData);

private:
    struct NeteaseSong
    {
        double id;
        double duration;
        QString time;
        double album;
        QString albumName;
        QString name;
        QList<double> artist;
        QStringList artistName;
        QString artistTotal;
    };
    QList<NeteaseSong> m_resultList;

    //Network access:
    QScopedPointer<QNetworkAccessManager> m_searchManager;

    //The search result widget.
    QScopedPointer<QStandardItemModel> m_model;
};

#endif // KNMUSICONLINENETEASE_H
