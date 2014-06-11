#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QTime>

#include "knmusicsearchresult.h"

#include "knmusiconlinenetease.h"

KNMusicOnlineNetease::KNMusicOnlineNetease(QObject *parent) :
    KNMusicOnlineBase(parent)
{
}

void KNMusicOnlineNetease::searchMusic(const QString &title)
{
    //Clear the model
    m_model.reset(new QStandardItemModel);
    //Initial request
    QNetworkRequest searchRequest;
    //Netease will use this cookie to certify your identity.
    QVariant versionCookieData;
    //Set the Netease Cloud Music search url.
    searchRequest.setUrl(QUrl("http://music.163.com/api/search/get"));
    //Set ID cookie, send this cookie every connect.
    QNetworkCookie versionCookie;
    versionCookie.setName("appver");
    versionCookie.setValue("1.5.2");
    versionCookieData.setValue(versionCookie);
    //Set the cookie to request header.
    searchRequest.setHeader(QNetworkRequest::CookieHeader,
                            versionCookieData);
    //Set the other request header.
    searchRequest.setHeader(QNetworkRequest::ContentTypeHeader,
                            "application/x-www-form-urlencoded");
    //The music search parameter:
    QByteArray searchParams;
    searchParams.append("limit=10&type=1&offset=0&sub=false&s=" + title);
    searchRequest.setHeader(QNetworkRequest::ContentLengthHeader,
                            searchParams.length());
    if(!m_searchManager.isNull())
    {
        disconnect(m_searchManager.data(), SIGNAL(finished(QNetworkReply*)),
                   this, SLOT(handleReplyData(QNetworkReply*)));
    }
    m_searchManager.reset(new QNetworkAccessManager);
    connect(m_searchManager.data(), SIGNAL(finished(QNetworkReply*)),
            this, SLOT(handleReplyData(QNetworkReply*)));
    //Do post.
    m_searchManager->clearAccessCache();
    m_searchManager->post(searchRequest, searchParams);
}

QStandardItemModel *KNMusicOnlineNetease::model() const
{
    return m_model.data();
}

void KNMusicOnlineNetease::handleReplyData(QNetworkReply *replyData)
{
    //Translate reply data to json document.
    QJsonDocument response=QJsonDocument::fromJson(replyData->readAll());
    if(response.isNull())
    {
        qDebug()<<"Error! No JSON data!";
        return;
    }
    qDebug()<<response.toJson();
    //Get the result object.
    QJsonObject replyObject=response.object(),
                songListObject=replyObject["result"].toObject();
    //"songCount" means how many songs have been found.
//    qDebug()<<songListObject["songCount"].toDouble();
    //Get the song list.
    QJsonArray songList=songListObject["songs"].toArray();
    //Analysis every song.
    for(int i=0; i<songList.size(); i++)
    {
        QJsonObject currentSong=songList.at(i).toObject();
        NeteaseSong currentSongData;
        //Name of the song.
        currentSongData.name=currentSong["name"].toString();
        //ID of the song.
        currentSongData.id=currentSong["id"].toDouble();
        //Duration of the song.
        currentSongData.duration=currentSong["duration"].toDouble();
        //Calculate the mm:ss format duration from the MS:
        int secondDuration=currentSongData.duration/1000;
        currentSongData.time = QString::number(secondDuration/60) + ":"
                + QString::number(secondDuration%60);
        //Album information.
        QJsonObject currentAlbum=currentSong["album"].toObject();
        currentSongData.album=currentAlbum["id"].toDouble();
        currentSongData.albumName=currentAlbum["name"].toString();
        //Artist information.
        QJsonArray currentArtists=currentSong["artists"].toArray();
        for(int j=0, artistCount=currentArtists.size(); j<artistCount; j++)
        {
            QJsonObject artistObject=currentArtists.at(j).toObject();
            currentSongData.artist.append(artistObject["id"].toDouble());
            QString artistName=artistObject["name"].toString();
            currentSongData.artistName.append(artistName);
            currentSongData.artistTotal.append(j==0?artistName:" / "+artistName);
        }
        QStandardItem *item=new QStandardItem(currentSongData.name + "<br />"+
                                              currentSongData.time + "<br />" +
                                              currentSongData.artistTotal + "<br />" +
                                              currentSongData.albumName);
        m_model->appendRow(item);
    }
}
