#include <QDir>
#include <QApplication>
#include <QTextCodec>
#include <QFileInfoList>
#include <QTimer>

#include <QDebug>

#include "knlibbass.h"

KNLibBass::KNLibBass(QObject *parent) :
    QObject(parent)
{
    //Initial Bass
    //Check bass version.
    if(HIWORD(BASS_GetVersion())!=BASSVERSION)
    {
        qDebug()<<"Bass version error!";
    }
    //Set Configure, I don't know what's this use.
    BASS_SetConfig(BASS_CONFIG_FLOATDSP,TRUE);
    if(!BASS_Init(-1,44100,0,NULL,NULL))
    {
        qDebug()<<"Cannot init bass library.";
        return;
    }
    //Check floatable.
    m_floatable=BASS_StreamCreate(44100,2,BASS_SAMPLE_FLOAT,NULL,0);
    if(m_floatable)
    {
        //Support floatable
        BASS_StreamFree(m_floatable);
        m_floatable=BASS_SAMPLE_FLOAT;
    }
    //Load Plugins
    loadPlugins();

    //Initial the position updater
    m_positionUpdater=new QTimer;
    m_positionUpdater->setInterval(200);
    connect(m_positionUpdater, &QTimer::timeout,
            [=]{emit positionChanged(BASS_ChannelBytes2Seconds(m_channel,
                                                               BASS_ChannelGetPosition(m_channel,BASS_POS_BYTE)));
    });

    //Initial UTF-8 Codec
    m_utf8codec=QTextCodec::codecForName("UTF-8");
}

KNLibBass::~KNLibBass()
{
    m_positionUpdater->deleteLater();
    BASS_PluginFree(0);
    BASS_Free();
}

void KNLibBass::loadMusic(const QString &filePath)
{
    //Free the main stream
    BASS_MusicFree(m_channel);
    BASS_StreamFree(m_channel);
    //Load the file.
    std::wstring uniPath=filePath.toStdWString();
    if(!(m_channel=BASS_StreamCreateFile(FALSE,uniPath.data(),0,0,BASS_SAMPLE_LOOP|BASS_UNICODE|m_floatable))
       && !(m_channel=BASS_MusicLoad(FALSE,uniPath.data(),0,0,BASS_SAMPLE_LOOP|BASS_UNICODE|BASS_MUSIC_RAMPS|m_floatable,1)))
    {
        qDebug()<<"Can't play the file.";
        return;
    }
    //Get the channel information.
    BASS_ChannelGetInfo(m_channel,&m_currentChannelInfo);
    //Get the byte duration.
    m_byteDuration=BASS_ChannelGetLength(m_channel, BASS_POS_BYTE);
    m_duration=BASS_ChannelBytes2Seconds(m_channel, m_byteDuration);
}

quint32 KNLibBass::duration() const
{
    return m_duration;
}

void KNLibBass::play()
{
    m_positionUpdater->start();
    BASS_ChannelPlay(m_channel, FALSE);
}

int KNLibBass::volume() const
{
    return BASS_GetVolume();
}

void KNLibBass::setVolume(const int &volumeSize)
{
    BASS_SetConfig(BASS_CONFIG_GVOL_STREAM, volumeSize);
}

void KNLibBass::setPosition(const int &secondPosition)
{
    BASS_ChannelSetPosition(m_channel,
                            BASS_ChannelSeconds2Bytes(m_channel, secondPosition),
                            BASS_POS_BYTE);
}

void KNLibBass::loadPlugins()
{
    //Load all bass plugins
    QDir pluginDir(QApplication::applicationDirPath()+"/Plugins");
    QFileInfoList pluginList=pluginDir.entryInfoList();
    while(!pluginList.isEmpty())
    {
        QFileInfo currentInfo=pluginList.takeFirst();
        //For Windows, the plugin suffix is dll. load all dll files.
        if(currentInfo.isFile() && currentInfo.suffix().toLower()=="dll")
        {
            HPLUGIN plug;
            //If we can load the plugin, output the data.
            if(plug=BASS_PluginLoad(currentInfo.absoluteFilePath().toStdWString().data(), 0))
            {
                const BASS_PLUGININFO *pinfo=BASS_PluginGetInfo(plug);
                //formatc -> Format count
                for(int i=0; i<pinfo->formatc; i++)
                {
                    qDebug()<<pinfo->formats[i].exts;
                }
            }
        }
    }
}
