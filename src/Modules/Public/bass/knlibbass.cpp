#include <QDir>
#include <QApplication>
#include <QFileInfoList>
#include <QTimer>

#include <QDebug>

#include "knlibbass.h"

KNLibBass::KNLibBass(QObject *parent) :
    KNMusicBackend(parent)
{
    //Connect the signals.
    connect(m_main.positionUpdater, &QTimer::timeout,
            [=]{DWORD currentPos=
                    BASS_ChannelBytes2Seconds(m_main.channel,
                                              BASS_ChannelGetPosition(m_main.channel, BASS_POS_BYTE));
                emit positionChanged(currentPos);
                if(currentPos==m_main.duration)
                {
                    stop();
                    m_main.stopped=true;
                    emit finished();
                }
                });
    connect(m_preview.positionUpdater, &QTimer::timeout,
            [=]{DWORD currentPos=BASS_ChannelBytes2Seconds(m_preview.channel,
                                                             BASS_ChannelGetPosition(m_preview.channel,BASS_POS_BYTE));
                emit previewPositionChanged(currentPos);
                if(currentPos==m_preview.duration)
                {
                    stopPreview();
                    m_preview.stopped=true;
                    emit previewFinished();
                }
                });
    connect(m_onlinePreivew.bufferCheck, &QTimer::timeout,
            this, &KNLibBass::onActionBufferCheckTimeout);

    //Initial Dymantic Link Library suffix
#ifdef Q_OS_WIN32
    m_dylinkSuffix="dll";
#endif
#ifdef Q_OS_MACX
    m_dylinkSuffix="dylib";
#endif
#ifdef Q_OS_LINUX
    m_dylinkSuffix="so";
#endif
    //Initial Bass
    //Check bass version.
    if(HIWORD(BASS_GetVersion())!=BASSVERSION)
    {
        qDebug()<<"Bass version error!";
    }
    //Set Configure, I don't know what's this use.
    BASS_SetConfig(BASS_CONFIG_FLOATDSP, TRUE);
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
    //Enable playlist processing
    BASS_SetConfig(BASS_CONFIG_NET_PLAYLIST,1);
    //Minimize automatic pre-buffering, so we can do it (and display it) instead
    BASS_SetConfig(BASS_CONFIG_NET_PREBUF,0);
    //Disable proxy
    BASS_SetConfigPtr(BASS_CONFIG_NET_PROXY,NULL);
    //Load Plugins
    loadPlugins();
}

KNLibBass::~KNLibBass()
{
    BASS_PluginFree(0);
    BASS_Free();
}

void KNLibBass::loadMusic(const QString &filePath)
{
    m_main.filePath=filePath;
    m_main.positionUpdater->stop();
    loadMusicFile(m_main);
    BASS_ChannelFlags(m_main.channel, 0, BASS_SAMPLE_LOOP);
    loadEQ();
}

void KNLibBass::loadPreview(const QString &filePath)
{
    m_preview.filePath=filePath;
    m_preview.positionUpdater->stop();
    loadMusicFile(m_preview);
    BASS_ChannelFlags(m_preview.channel, 0, BASS_SAMPLE_LOOP);
}

void KNLibBass::loadUrl(const QString &url)
{
//    DWORD r;
    m_onlinePreivew.filePath=url;
    m_onlinePreivew.positionUpdater->stop();
    BASS_StreamFree(m_onlinePreivew.channel);
    qDebug()<<m_onlinePreivew.filePath.toStdString().data();
    m_onlinePreivew.channel=BASS_StreamCreateURL(m_onlinePreivew.filePath.toStdString().data(),
                                                 0,
                                                 BASS_STREAM_BLOCK|BASS_STREAM_STATUS|BASS_STREAM_AUTOFREE,
                                                 NULL,
                                                 NULL);
    if(!m_onlinePreivew.channel)
    {
        qDebug()<<"Cannot play the channel";
    }
    else
    {
        m_onlinePreivew.bufferCheck->start();
    }
}

bool KNLibBass::loadInfoCollect(const QString &filePath)
{
#ifdef Q_OS_WIN32
    std::wstring uniPath=filePath.toStdWString();
    if(m_infoCollector.channel=BASS_StreamCreateFile(FALSE,uniPath.data(),0,0,BASS_MUSIC_DECODE|BASS_UNICODE))
#endif
#ifdef Q_OS_UNIX
    std::string uniPath=filePath.toStdString();
    if(m_infoCollector.channel=BASS_StreamCreateFile(FALSE,uniPath.data(),0,0,BASS_MUSIC_DECODE))
#endif
    {
        QWORD byteLength=
                BASS_ChannelGetLength(m_infoCollector.channel, BASS_POS_BYTE);
        m_infoCollector.duration=
                BASS_ChannelBytes2Seconds(m_infoCollector.channel,
                                          byteLength);
        // Bitrate (Kbps)
        m_infoCollector.bitrate=(DWORD)(BASS_StreamGetFilePosition(m_infoCollector.channel, BASS_FILEPOS_END)/
                              (125*m_infoCollector.duration)+0.5);
        //Get channel info(for sampling rate)
        BASS_ChannelGetInfo(m_infoCollector.channel,&m_infoCollector.channelInfo);
        BASS_StreamFree(m_infoCollector.channel);
        return true;
    }
    return false;
}

float KNLibBass::duration() const
{
    return m_main.duration;
}

float KNLibBass::position() const
{
    return BASS_ChannelBytes2Seconds(m_main.channel,
                                     BASS_ChannelGetPosition(m_main.channel,BASS_POS_BYTE));
}

float KNLibBass::previewDuration() const
{
    return m_preview.duration;
}

void KNLibBass::play()
{
    m_main.positionUpdater->start();
    if(m_main.stopped)
    {
        m_main.stopped=false;
        BASS_ChannelPlay(m_main.channel, TRUE);
    }
    else
    {
        BASS_ChannelPlay(m_main.channel, FALSE);
    }
}

void KNLibBass::playFile(const QString &fileName)
{
    loadMusic(fileName);
    emit durationChanged(duration());
    play();
}

void KNLibBass::playPreview()
{
    float mainVolume;
    BASS_ChannelGetAttribute(m_main.channel,
                             BASS_ATTRIB_VOL,
                             &mainVolume);
    m_originalVolume=mainVolume;
    if(m_originalVolume>0.1)
    {
        BASS_ChannelSetAttribute(m_preview.channel,
                                 BASS_ATTRIB_VOL,
                                 m_originalVolume);
        BASS_ChannelSetAttribute(m_main.channel,
                                 BASS_ATTRIB_VOL,
                                 m_originalVolume/8);
    }
    else
    {
        m_originalVolume=-1;
    }
    m_preview.positionUpdater->start();
    if(m_preview.stopped)
    {
        m_preview.stopped=false;
        BASS_ChannelPlay(m_preview.channel, TRUE);
    }
    else
    {
        BASS_ChannelPlay(m_preview.channel, FALSE);
    }
}

void KNLibBass::stop()
{
    BASS_ChannelStop(m_main.channel);
    m_main.positionUpdater->stop();
    setPosition(0);
    emit stopped();
}

void KNLibBass::stopPreview()
{
    BASS_ChannelStop(m_preview.channel);
    if(m_originalVolume>0)
    {
        BASS_ChannelSetAttribute(m_main.channel,
                                 BASS_ATTRIB_VOL,
                                 m_originalVolume);
        m_originalVolume=-1;
    }
    m_preview.positionUpdater->stop();
}

void KNLibBass::pause()
{
    BASS_ChannelPause(m_main.channel);
    m_main.positionUpdater->stop();
}

void KNLibBass::pausePreview()
{
    BASS_ChannelPause(m_preview.channel);
    if(m_originalVolume>0)
    {
        BASS_ChannelSetAttribute(m_main.channel,
                                 BASS_ATTRIB_VOL,
                                 m_originalVolume);
        m_originalVolume=-1;
    }
    m_preview.positionUpdater->stop();
}

void KNLibBass::getFFTData(float *fftData)
{
    BASS_ChannelGetData(m_main.channel,
                        fftData,
                        BASS_DATA_FFT2048);
}

float KNLibBass::volume() const
{
    return BASS_GetConfig(BASS_CONFIG_GVOL_STREAM);
}

int KNLibBass::collectorDuration() const
{
    return m_infoCollector.duration;
}

int KNLibBass::collectorBitrate() const
{
    return m_infoCollector.bitrate;
}

int KNLibBass::collectorSamplingRate() const
{
    return m_infoCollector.channelInfo.freq;
}

void KNLibBass::setVolume(const float &volumeSize)
{
    BASS_SetConfig(BASS_CONFIG_GVOL_STREAM, volumeSize);
}

void KNLibBass::setPosition(const float &secondPosition)
{
    BASS_ChannelSetPosition(m_main.channel,
                            BASS_ChannelSeconds2Bytes(m_main.channel, secondPosition),
                            BASS_POS_BYTE);
}

void KNLibBass::setPreviewPosition(const float &secondPosition)
{
    BASS_ChannelSetPosition(m_preview.channel,
                            BASS_ChannelSeconds2Bytes(m_preview.channel, secondPosition),
                            BASS_POS_BYTE);
}

void KNLibBass::setEqualizerParam(const int &index, const float &value)
{
    m_eqGain[index]=value;
    BASS_DX8_PARAMEQ equalizerParam;
    BASS_FXGetParameters(m_equalizer[index], &equalizerParam);
    equalizerParam.fGain=m_eqGain[index];
    BASS_FXSetParameters(m_equalizer[index], &equalizerParam);
}

void KNLibBass::onActionBufferCheckTimeout()
{
    m_onlinePreivew.progress=BASS_StreamGetFilePosition(m_onlinePreivew.channel,
                                                        BASS_FILEPOS_BUFFER)*100/
                             BASS_StreamGetFilePosition(m_onlinePreivew.channel,BASS_FILEPOS_END);
    qDebug()<<BASS_StreamGetFilePosition(m_onlinePreivew.channel,BASS_FILEPOS_END);
    if(m_onlinePreivew.progress>75 ||
            !BASS_StreamGetFilePosition(m_onlinePreivew.channel,BASS_FILEPOS_CONNECTED))
    {
        m_onlinePreivew.bufferCheck->stop();
        BASS_ChannelPlay(m_onlinePreivew.channel,FALSE);
    }
    else
    {
        //Buffering.
        ;
    }
}

void CALLBACK KNLibBass::StatusProc(const void *buffer, DWORD length, void *user)
{
    ;
}

void CALLBACK KNLibBass::MetaSync(HSYNC handle, DWORD channel, DWORD data, void *user)
{
    ;
}

void KNLibBass::loadMusicFile(MusicThread &musicThread)
{
    //Free the main stream
    BASS_MusicFree(musicThread.channel);
    BASS_StreamFree(musicThread.channel);
    //Load the file.
#ifdef Q_OS_WIN32
    std::wstring uniPath=musicThread.filePath.toStdWString();
    if(!(musicThread.channel=BASS_StreamCreateFile(FALSE,uniPath.data(),0,0,BASS_SAMPLE_LOOP|BASS_UNICODE|m_floatable))
       && !(musicThread.channel=BASS_MusicLoad(FALSE,uniPath.data(),0,0,BASS_SAMPLE_LOOP|BASS_UNICODE|BASS_MUSIC_RAMPS|m_floatable,1)))
#endif
#ifdef Q_OS_UNIX
    std::string uniPath=musicThread.filePath.toStdString();
    if(!(musicThread.channel=BASS_StreamCreateFile(FALSE,uniPath.data(),0,0,BASS_SAMPLE_LOOP|m_floatable))
       && !(musicThread.channel=BASS_MusicLoad(FALSE,uniPath.data(),0,0,BASS_SAMPLE_LOOP|BASS_MUSIC_RAMPS|m_floatable,1)))
#endif
    {
        qDebug()<<"Can't play the file.";
        return;
    }
    //Get the channel information.
    BASS_ChannelGetInfo(musicThread.channel,&musicThread.channelInfo);
    //Get the byte duration.
    musicThread.byteDuration=BASS_ChannelGetLength(musicThread.channel, BASS_POS_BYTE);
    musicThread.duration=BASS_ChannelBytes2Seconds(musicThread.channel, musicThread.byteDuration);
}

void KNLibBass::loadPlugins()
{
    //Load all bass plugins
    QDir pluginDir(QApplication::applicationDirPath()+"/Plugins");
    QFileInfoList pluginList=pluginDir.entryInfoList();
    while(!pluginList.isEmpty())
    {
        QFileInfo currentInfo=pluginList.takeFirst();
        if(currentInfo.isFile() && currentInfo.suffix().toLower()==m_dylinkSuffix)
        {
            HPLUGIN plug;
            //If we can load the plugin, output the data.
#ifdef Q_OS_WIN32
            plug=BASS_PluginLoad(currentInfo.absoluteFilePath().toStdWString().data(), 0);
#endif
#ifdef Q_OS_UNIX
            plug=BASS_PluginLoad(currentInfo.absoluteFilePath().toLocal8Bit().data(), 0);
#endif
            if(plug)
            {
//                const BASS_PLUGININFO *pinfo=BASS_PluginGetInfo(plug);
//                formatc -> Format count
//                for(DWORD i=0; i<pinfo->formatc; i++)
//                {
//                    qDebug()<<pinfo->formats[i].exts;
//                }
            }
        }
    }
}

void KNLibBass::loadEQ()
{
    for(int i=0; i<EqualizerCount; i++)
    {
        BASS_DX8_PARAMEQ equalizerParams;
        m_equalizer[i]=BASS_ChannelSetFX(m_main.channel,
                                         BASS_FX_DX8_PARAMEQ,
                                         0);
        equalizerParams.fGain=m_eqGain[i];
        equalizerParams.fCenter=m_eqFrequency[i];
        equalizerParams.fBandwidth=m_eqBandWidth[i];
        BASS_FXSetParameters(m_equalizer[i], &equalizerParams);
    }
}
