#ifndef KNSTDLIBONLINESESSION_H
#define KNSTDLIBONLINESESSION_H

#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QNetworkCookie>
#include <QScopedPointer>

#include "knlibonlinesession.h"

class KNStdLibOnlineSession : public KNLibOnlineSession
{
    Q_OBJECT
public:
    explicit KNStdLibOnlineSession(QObject *parent = 0);
    bool working() const;

signals:
    void dataReplied(const QByteArray &replyData);

public slots:
    void post(const QNetworkRequest &request, const QByteArray &data);
    void get(const QNetworkRequest &request);

private slots:
    void handleReplyData(QNetworkReply *replyData);

private:
    void resetSearchManager();
    //Network access manager:
    QScopedPointer<QNetworkAccessManager> m_searchManager;
    bool m_working=false;
};

#endif // KNSTDLIBONLINESESSION_H
