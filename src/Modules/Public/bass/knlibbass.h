#ifndef KNLIBBASS_H
#define KNLIBBASS_H

#include <stdio.h>
#include "bass.h"

//All kinds of basses
#include "bass_aac.h"
#include "bass_ac3.h"
#include "bass_aix.h"
#include "bass_alac.h"
#include "bass_ape.h"
#include "bass_fx.h"
#include "bass_mpc.h"
#include "bass_ofr.h"
#include "bass_spx.h"
#include "bass_tta.h"
#include "basscd.h"
#include "bassenc.h"
#include "bassflac.h"
#include "bassmidi.h"
#include "bassmix.h"
#include "bassopus.h"
#include "basswma.h"
#include "basswv.h"
#include "bpm.h"
#include "dsp.h"
#include "reverse.h"
#include "tempo.h"

#include <QObject>

class QTimer;
class QTextCodec;
class KNLibBass : public QObject
{
    Q_OBJECT
public:
    explicit KNLibBass(QObject *parent = 0);
    ~KNLibBass();
    void loadMusic(const QString &filePath);
    quint32 duration() const;
    void play();
    int volume() const;

signals:
    void positionChanged(quint32 position);

public slots:
    void setVolume(const int &volumeSize);
    void setPosition(const int &secondPosition);

private:
    void loadPlugins();
    DWORD m_channel, m_floatable, m_duration;
    BASS_CHANNELINFO m_currentChannelInfo;
    QWORD m_byteDuration;
    QTimer *m_positionUpdater;
    QTextCodec *m_utf8codec;
};

#endif // KNLIBBASS_H
