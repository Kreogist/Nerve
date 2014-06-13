#include "knstdlibonlinesession.h"

KNStdLibOnlineSession::KNStdLibOnlineSession(QObject *parent) :
    KNLibOnlineSession(parent)
{
}

bool KNStdLibOnlineSession::working() const
{
    return m_working;
}

void KNStdLibOnlineSession::post(const QNetworkRequest &request, const QByteArray &data)
{
    resetSearchManager();
    m_searchManager->post(request, data);
    m_working=true;
}

void KNStdLibOnlineSession::handleReplyData(QNetworkReply *replyData)
{
    m_working=false;
    emit dataReplied(replyData->readAll());
}

void KNStdLibOnlineSession::resetSearchManager()
{
    //If the search manager is not null, delete it.
    if(!m_searchManager.isNull())
    {
        disconnect(m_searchManager.data(), &QNetworkAccessManager::finished,
                   this, &KNStdLibOnlineSession::handleReplyData);
    }
    m_searchManager.reset(new QNetworkAccessManager);
    connect(m_searchManager.data(), &QNetworkAccessManager::finished,
            this, &KNStdLibOnlineSession::handleReplyData);
    m_searchManager->clearAccessCache();
}
