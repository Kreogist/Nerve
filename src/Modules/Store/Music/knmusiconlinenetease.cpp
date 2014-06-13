#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QCryptographicHash>
#include <QTime>

#include "../../Base/knstdlibonlinesessionmanager.h"
#include "knmusiconlinenetease.h"

KNMusicOnlineNetease::KNMusicOnlineNetease(QObject *parent) :
    KNMusicOnlineBase(parent)
{
    m_accessManager=new KNStdLibOnlineSessionManager(this);
    connect(m_accessManager, &KNStdLibOnlineSessionManager::postDataUpdate,
            this, &KNMusicOnlineNetease::handlePostData);
}

void KNMusicOnlineNetease::searchMusic(const QString &title)
{
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
    m_accessManager->post(searchRequest, searchParams);
}

QStandardItemModel *KNMusicOnlineNetease::model() const
{
    return m_model.data();
}

void KNMusicOnlineNetease::handlePostData()
{
    //Clear the model
    m_model.reset(new QStandardItemModel);
    //Translate reply data to json document.
    QJsonDocument response=QJsonDocument::fromJson(m_accessManager->takeReplyData());
    if(response.isNull())
    {
        qDebug()<<"Error! No JSON data!";
        return;
    }
//    qDebug()<<response.toJson();
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
        QList<QStandardItem *> rowData;
        QStandardItem *item=new QStandardItem(currentSongData.name);
        item->setEditable(false);
        rowData.append(item);
        item=new QStandardItem(currentSongData.artistTotal);
        item->setEditable(false);
        rowData.append(item);
        item=new QStandardItem(currentSongData.albumName);
        item->setEditable(false);
        rowData.append(item);
        item=new QStandardItem(currentSongData.time);
        item->setEditable(false);
        rowData.append(item);
        m_model->appendRow(rowData);
    }
    emit modelUpdate();
}

QByteArray encrypted_id(QByteArray id)
{
    QByteArray byte1=QByteArray("3go8&$8*3*3h0k(2)2");
    QByteArray byte2=id;
    QByteArray result;
    int byte1_len = byte1.length();
    for(int i=0; i<byte2.length(); i++)
        result.append(byte2.at(i)^byte1.at(i%byte1_len));
    result=QCryptographicHash::hash(result, QCryptographicHash::Md5);
    result=result.toBase64();
    result = result.replace('/', '_');
    result = result.replace('+', '-');
    return result;
}
