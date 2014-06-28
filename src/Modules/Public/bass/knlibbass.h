#ifndef KNLIBBASS_H
#define KNLIBBASS_H

#include <stdio.h>

#include <QTimer>

#include "../Base/knmusicbackend.h"

//Include all public libraries.
#include "bass.h"
#include "bass_ac3.h"
#include "bass_ape.h"
#include "bass_fx.h"
#include "bass_mpc.h"
#include "bass_spx.h"
#include "bass_tta.h"
#include "bassenc.h"
#include "bassflac.h"
#include "bassmidi.h"
#include "bassmix.h"
#include "basswv.h"
#include "bassopus.h"

#ifdef Q_OS_WIN32
//Include all win32 bass libraries
#include "bass_aac.h"
#include "bass_aix.h"
#include "bass_alac.h"
#include "bass_ofr.h"
#include "basscd.h"
#include "basswma.h"
#include "bpm.h"
#include "dsp.h"
#include "reverse.h"
#include "tempo.h"
#endif

#ifdef Q_OS_MACX
#include "bass_vst.h"
#endif

#ifdef Q_OS_LINUX
//Include all linux bass libraries.
#include "bass_aac.h"
#include "bass_alac.h"
#include "basscd.h"
#endif

class KNLibBass : public KNMusicBackend
{
    Q_OBJECT
public:
    explicit KNLibBass(QObject *parent = 0);
    ~KNLibBass();
    void loadMusic(const QString &filePath);
    void loadPreview(const QString &filePath);
    void loadUrl(const QString &url);
    bool loadInfoCollect(const QString &filePath);
    float duration() const;
    float position() const;
    float previewDuration() const;
    void play();
    void playPreview();
    void stop();
    void stopPreview();
    void pause();
    void pausePreview();
    void getFFTData(float *fftData);
    float volume() const;

    int collectorDuration() const;
    int collectorBitrate() const;
    int collectorSamplingRate() const;

signals:

public slots:
    void setVolume(const float &volumeSize);
    void setPosition(const float &secondPosition);
    void setPreviewPosition(const float &secondPosition);
    void setEqualizerParam(const int &index,
                           const float &value);

private slots:
    void onActionBufferCheckTimeout();

private:
    struct MusicThread
    {
        QString filePath;
        bool stopped=true;
        DWORD channel;
        DWORD duration;
        BASS_CHANNELINFO channelInfo;
        QWORD byteDuration;
        QTimer *positionUpdater;
        MusicThread()
        {
            positionUpdater=new QTimer;
            positionUpdater->setInterval(30);
        }
        ~MusicThread()
        {
            if(positionUpdater->parent()==0)
            {
                positionUpdater->deleteLater();
            }
        }
    };

    struct InfoCollectThread
    {
        DWORD channel;
        DWORD duration;
        DWORD bitrate;
        BASS_CHANNELINFO channelInfo;
        QByteArray fingerPrint;
    };

    struct OnlineThread
    {
        QString filePath;
        bool stopped=true;
        DWORD channel;
        DWORD progress;
        DWORD duration;
        BASS_CHANNELINFO channelInfo;
        QWORD byteDuration;
        QTimer *positionUpdater,
               *bufferCheck;
        OnlineThread()
        {
            positionUpdater=new QTimer;
            positionUpdater->setInterval(30);
            bufferCheck=new QTimer;
            bufferCheck->setInterval(50);
        }
        ~OnlineThread()
        {
            if(positionUpdater->parent()==0)
            {
                positionUpdater->deleteLater();
            }
            if(bufferCheck->parent()==0)
            {
                bufferCheck->deleteLater();
            }
        }
    };

    HFX m_equalizer[EqualizerCount];
    void CALLBACK StatusProc(const void *buffer, DWORD length, void *user);
    void CALLBACK MetaSync(HSYNC handle, DWORD channel, DWORD data, void *user);
    float m_eqGain[EqualizerCount]={0};
    MusicThread m_main, m_preview;
    OnlineThread m_onlinePreivew;
    InfoCollectThread m_infoCollector;
    void loadMusicFile(MusicThread &musicThread);
    void loadPlugins();
    void loadEQ();
    bool m_firstLoadFile=true;
    DWORD m_floatable;
    QString m_dylinkSuffix;
    float m_originalVolume=-1;
};

#endif // KNLIBBASS_H
