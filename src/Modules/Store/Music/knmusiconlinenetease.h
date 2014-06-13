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
    void handleListData();
    void handleMusicData();

private:
    void fetchSongData(const QString &songID, const int &songIndex);
    QByteArray encrypted_id(QByteArray id);
    struct NeteaseSong
    {
        int id;
        int duration;
        QString time;
        int album;
        QString albumName;
        QString name;
        QList<int> artist;
        QStringList artistName;
        QString artistTotal;
        QString dfsId;
        QString url;
    };
    QList<NeteaseSong> m_resultList;

    //The search result widget.
    QScopedPointer<QStandardItemModel> m_model;

    //The network access manager.
    KNStdLibOnlineSessionManager *m_accessManager;
};

#endif // KNMUSICONLINENETEASE_H
