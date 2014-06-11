#ifndef KNMUSICEQ_H
#define KNMUSICEQ_H

#include <QWidget>

#include "../../Public/bass/knlibbass.h"

class QSignalMapper;
class QLabel;
class KNLabelEditor;
class KNMusicVolumeSlider;
class KNLibBass;
class KNMusicEQSlider;
class KNMusicEQ : public QWidget
{
    Q_OBJECT
public:
    explicit KNMusicEQ(KNLibBass *backend, QWidget *parent = 0);
    void setBackend(KNLibBass *backend);

signals:

public slots:
    void retranslate();
    void retranslateAndSet();

private slots:
    void setEQParam(const int &index);
    void setTuneValue(const int &index);

private:
    KNLibBass *m_bassBackend;
    QSignalMapper *m_tuneRelease, *m_textChange;
    KNLabelEditor *m_equalizerEdit[KNLibBass::EqualizerCount],
                  *m_rangeEdit;
    KNMusicEQSlider *m_equalizerTune[KNLibBass::EqualizerCount],
                    *m_rangeControl;
    QLabel *m_equalizerCaption[KNLibBass::EqualizerCount],
           *m_rangeLabel;
    QString m_rangeCaption;
};

#endif // KNMUSICEQ_H
