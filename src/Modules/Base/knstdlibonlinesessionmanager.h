#ifndef KNSTDLIBONLINESESSIONMANAGER_H
#define KNSTDLIBONLINESESSIONMANAGER_H

#include <QThread>
#include <QNetworkRequest>
#include <QByteArray>
#include <QList>

#include "knlibonlinesessionmanager.h"

class KNStdLibOnlineSession;
class KNStdLibOnlineSessionManager : public KNLibOnlineSessionManager
{
    Q_OBJECT
public:
    explicit KNStdLibOnlineSessionManager(QObject *parent = 0);
    ~KNStdLibOnlineSessionManager();
    NetworkReply takeReplyData();
    void post(const QNetworkRequest &request,
              const QByteArray &data,
              const QVariant &id=QVariant());
    void get(const QNetworkRequest &request,
             const QVariant &id=QVariant());

signals:
    void postDataUpdate();
    void getDataUpdate();
    void doPost(const QNetworkRequest &request,
                const QByteArray &data,
                const QVariant &id);
    void doGet(const QNetworkRequest &request,
               const QVariant &id);

public slots:

private slots:
    void onActionDataReplied(const QByteArray &data);

private:
    void processRequest();
    enum NetworkRequestType
    {
        Post,
        Get
    };

    struct NetworkRequest
    {
        int type;
        QNetworkRequest request;
        QByteArray data;
        QVariant id;
    };

    QList<NetworkRequest> m_requestList;
    QList<NetworkReply> m_replyList;
    KNStdLibOnlineSession *m_session;
    QThread m_sessionThread;
};

#endif // KNSTDLIBONLINESESSIONMANAGER_H
