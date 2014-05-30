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
    explicit KNLibBass(QObject *parent = 0);
    ~KNLibBass();
    void loadMusic(const QString &filePath);
    void loadPreview(const QString &filePath);
    quint32 duration() const;
    quint32 previewDuration() const;
    void play();
    void playPreview();
    void stop();
    void stopPreview();
    void pausePreview();
    int volume() const;

signals:
    void positionChanged(quint32 position);
    void previewPositionChanged(quint32 position);

public slots:
    void setVolume(const int &volumeSize);
    void setPosition(const int &secondPosition);
    void setPreviewPosition(const int &secondPosition);

private:
    struct MusicThread
    {
        QString filePath;
        DWORD channel;
        DWORD duration;
        BASS_CHANNELINFO channelInfo;
        QWORD byteDuration;
        QTimer *positionUpdater;
        MusicThread()
        {
            positionUpdater=new QTimer;
            positionUpdater->setInterval(200);
        }
        ~MusicThread()
        {
            positionUpdater->deleteLater();
        }
    };
    MusicThread m_main, m_preview;
    void loadMusicFile(MusicThread &musicThread);
    void loadPlugins();
    DWORD m_floatable;
    QString m_dylinkSuffix;
    float m_originalVolume=-1;
};

#endif // KNLIBBASS_H
