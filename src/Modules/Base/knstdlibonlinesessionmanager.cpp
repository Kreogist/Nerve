#include "knstdlibonlinesession.h"

#include "knstdlibonlinesessionmanager.h"

KNStdLibOnlineSessionManager::KNStdLibOnlineSessionManager(QObject *parent) :
    KNLibOnlineSessionManager(parent)
{
    m_session=new KNStdLibOnlineSession;
    m_session->moveToThread(&m_sessionThread);
    connect(this, &KNStdLibOnlineSessionManager::doPost,
            m_session, &KNStdLibOnlineSession::post);
    connect(m_session, &KNStdLibOnlineSession::dataReplied,
            this, &KNStdLibOnlineSessionManager::onActionDataReplied);

    m_sessionThread.start();
}

KNStdLibOnlineSessionManager::~KNStdLibOnlineSessionManager()
{
    m_sessionThread.quit();
    m_sessionThread.wait();
    m_session->deleteLater();
}

QByteArray KNStdLibOnlineSessionManager::takeReplyData()
{
    return m_replyList.takeFirst();
}

void KNStdLibOnlineSessionManager::post(const QNetworkRequest &request,
                                        const QByteArray &data)
{
    NetworkRequest currentRequest;
    currentRequest.type=Post;
    currentRequest.request=request;
    currentRequest.data=data;
    m_requestList.append(currentRequest);
    processRequest();
}

void KNStdLibOnlineSessionManager::onActionDataReplied(const QByteArray &data)
{
    m_replyList.append(data);
    int requestType=m_requestList.first().type;
    m_requestList.removeFirst();
    switch(requestType)
    {
    case Post:
        emit postDataUpdate();
    }
    if(!m_requestList.isEmpty())
    {
        processRequest();
    }
}

void KNStdLibOnlineSessionManager::processRequest()
{
    if(m_session->working())
    {
        return;
    }
    NetworkRequest currentRequest=m_requestList.first();
    switch(currentRequest.type)
    {
    case Post:
        emit doPost(currentRequest.request, currentRequest.data);
        return;
    }
}
