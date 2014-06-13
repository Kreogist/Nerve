#include "knstdlibonlinesession.h"

#include "knstdlibonlinesessionmanager.h"

KNStdLibOnlineSessionManager::KNStdLibOnlineSessionManager(QObject *parent) :
    KNLibOnlineSessionManager(parent)
{
    m_session=new KNStdLibOnlineSession;
    m_session->moveToThread(&m_sessionThread);
    connect(this, &KNStdLibOnlineSessionManager::doPost,
            m_session, &KNStdLibOnlineSession::post);
    connect(this, &KNStdLibOnlineSessionManager::doGet,
            m_session, &KNStdLibOnlineSession::get);
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

KNLibOnlineSessionManager::NetworkReply KNStdLibOnlineSessionManager::takeReplyData()
{
    return m_replyList.takeFirst();
}

void KNStdLibOnlineSessionManager::post(const QNetworkRequest &request,
                                        const QByteArray &data,
                                        const QVariant &id)
{
    NetworkRequest currentRequest;
    currentRequest.type=Post;
    currentRequest.request=request;
    currentRequest.data=data;
    currentRequest.id=id;
    m_requestList.append(currentRequest);
    processRequest();
}

void KNStdLibOnlineSessionManager::get(const QNetworkRequest &request,
                                       const QVariant &id)
{
    NetworkRequest currentRequest;
    currentRequest.type=Get;
    currentRequest.request=request;
    currentRequest.id=id;
    m_requestList.append(currentRequest);
    processRequest();
}

void KNStdLibOnlineSessionManager::onActionDataReplied(const QByteArray &data)
{
    NetworkReply currentReply;
    NetworkRequest currentRequest=m_requestList.takeFirst();
    currentReply.id=currentRequest.id;
    currentReply.data=data;
    m_replyList.append(currentReply);
    switch(currentRequest.type)
    {
    case Post:
        emit postDataUpdate();
        break;
    case Get:
        emit getDataUpdate();
        break;
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
        emit doPost(currentRequest.request, currentRequest.data, currentRequest.id);
        return;
    case Get:
        emit doGet(currentRequest.request, currentRequest.id);
        return;
    }
}
