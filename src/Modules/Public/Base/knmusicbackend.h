#ifndef KNMUSICBACKEND_H
#define KNMUSICBACKEND_H

#include <QObject>

class KNMusicBackend : public QObject
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
    explicit KNMusicBackend(QObject *parent = 0);
    virtual void loadMusic(const QString &filePath)=0;
    virtual void loadPreview(const QString &filePath)=0;
    virtual void loadUrl(const QString &url)=0;
    virtual bool loadInfoCollect(const QString &filePath)=0;
    QString eqFrequencyTitle(const int &index);
    virtual float duration() const=0;
    virtual float position() const=0;
    virtual float previewDuration() const=0;
    virtual void play()=0;
    virtual void playFile(const QString &fileName)=0;
    virtual void playPreview()=0;
    virtual void stop()=0;
    virtual void stopPreview()=0;
    virtual void pause()=0;
    virtual void pausePreview()=0;
    virtual void getFFTData(float *fftData)=0;
    virtual float volume() const=0;

    virtual int collectorDuration() const=0;
    virtual int collectorBitrate() const=0;
    virtual int collectorSamplingRate() const=0;

signals:
    void positionChanged(float position);
    void durationChanged(float duration);
    void finished();
    void stopped();
    void previewPositionChanged(float position);
    void previewFinished();

public slots:
    virtual void setVolume(const float &volumeSize)=0;
    virtual void setPosition(const float &secondPosition)=0;
    virtual void setPreviewPosition(const float &secondPosition)=0;
    virtual void setEqualizerParam(const int &index,
                                   const float &value)=0;

protected:
    float m_eqFrequency[EqualizerCount]={
        32, 63, 125, 250, 500, 1000, 2000, 4000, 8000, 16000
    };
    float m_eqBandWidth[EqualizerCount]={
       31, 62, 188, 312, 688, 1312, 2688, 5312, 10688, 21312
    };
    QString m_eqTitle[EqualizerCount]={
       "32", "63", "125", "250", "500", "1k", "2k", "4k", "8k", "16k"
    };
};

#endif // KNMUSICBACKEND_H
