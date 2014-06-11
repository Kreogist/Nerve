#ifndef KNLIBBASS_H
#define KNLIBBASS_H

#include <stdio.h>

#include <QTimer>

#include <QObject>

#ifdef Q_OS_WIN32
//Include all win32 bass libraries
#include "win32/bass.h"
#include "win32/bass_aac.h"
#include "win32/bass_ac3.h"
#include "win32/bass_aix.h"
#include "win32/bass_alac.h"
#include "win32/bass_ape.h"
#include "win32/bass_fx.h"
#include "win32/bass_mpc.h"
#include "win32/bass_ofr.h"
#include "win32/bass_spx.h"
#include "win32/bass_tta.h"
#include "win32/basscd.h"
#include "win32/bassenc.h"
#include "win32/bassflac.h"
#include "win32/bassmidi.h"
#include "win32/bassmix.h"
#include "win32/bassopus.h"
#include "win32/basswma.h"
#include "win32/basswv.h"
#include "win32/bpm.h"
#include "win32/dsp.h"
#include "win32/reverse.h"
#include "win32/tempo.h"
#endif

#ifdef Q_OS_MACX
#include "macosx/bass.h"
#include "macosx/bass_ac3.h"
#include "macosx/bass_ape.h"
#include "macosx/bass_fx.h"
#include "macosx/bass_mpc.h"
#include "macosx/bass_spx.h"
#include "macosx/bass_tta.h"
#include "macosx/bassenc.h"
#include "macosx/bassflac.h"
#include "macosx/bassmidi.h"
#include "macosx/bassmix.h"
#include "macosx/bassopus.h"
#include "macosx/basswv.h"
#endif

class KNLibBass : public QObject
{
    Q_OBJECT
public:
    enum Equalizer31
    {
        Hz32,
        Hz63,
        Hz125,
        Hz250,
        Hz500,
        Hz1000,
        Hz2000,
        Hz4000,
        Hz8000,
        Hz16000,
        EqualizerCount
    };
    explicit KNLibBass(QObject *parent = 0);
    ~KNLibBass();
    void loadMusic(const QString &filePath);
    void loadPreview(const QString &filePath);
    bool loadInfoCollect(const QString &filePath);
    QString eqFrequencyTitle(const int &index);
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
    void positionChanged(float position);
    void finished();
    void stopped();
    void previewPositionChanged(float position);
    void previewFinished();

public slots:
    void setVolume(const float &volumeSize);
    void setPosition(const float &secondPosition);
    void setPreviewPosition(const float &secondPosition);
    void setEqualizerParam(const int &index,
                           const float &value);

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

    HFX m_equalizer[EqualizerCount];
    float m_eqFrequency[EqualizerCount]={
        32, 63, 125, 250, 500, 1000, 2000, 4000, 8000, 16000
    };
    float m_eqBandWidth[EqualizerCount]={
       31, 62, 188, 312, 688, 1312, 2688, 5312, 10688, 21312
    };
    QString m_eqTitle[EqualizerCount]={
       "32", "63", "125", "250", "500", "1k", "2k", "4k", "8k", "16k"
    };
    float m_eqGain[EqualizerCount]={0};
    MusicThread m_main, m_preview;
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
