#ifndef KNMUSICONLINENETEASE_H
#define KNMUSICONLINENETEASE_H

#include <QStandardItemModel>
#include "knmusiconlinebase.h"

class KNStdLibOnlineSessionManager;
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
    void handlePostData();

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

    //The search result widget.
    QScopedPointer<QStandardItemModel> m_model;

    //The network access manager.
    KNStdLibOnlineSessionManager *m_accessManager;
};

#endif // KNMUSICONLINENETEASE_H
