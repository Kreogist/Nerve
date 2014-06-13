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
    QByteArray takeReplyData();
    void post(const QNetworkRequest &request, const QByteArray &data);

signals:
    void postDataUpdate();
    void doPost(const QNetworkRequest &request, const QByteArray &data);

public slots:

private slots:
    void onActionDataReplied(const QByteArray &data);

private:
    void processRequest();
    enum NetworkRequestType
    {
        Post
    };

    struct NetworkRequest
    {
        int type;
        QNetworkRequest request;
        QByteArray data;
    };
    QList<NetworkRequest> m_requestList;
    QList<QByteArray> m_replyList;
    KNStdLibOnlineSession *m_session;
    QThread m_sessionThread;
};

#endif // KNSTDLIBONLINESESSIONMANAGER_H
