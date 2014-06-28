#ifndef KNMUSICEQ_H
#define KNMUSICEQ_H

#include <QWidget>

#include "../../Public/Base/KNMusicBackend.h"

class QSignalMapper;
class QLabel;
class KNLabelEditor;
class KNMusicVolumeSlider;
class KNMusicBackend;
class KNMusicEQSlider;
class KNMusicEQ : public QWidget
{
    Q_OBJECT
public:
    explicit KNMusicEQ(KNMusicBackend *backend, QWidget *parent = 0);

signals:

public slots:
    void retranslate();
    void retranslateAndSet();

private slots:
    void setEQParam(const int &index);
    void setTuneValue(const int &index);

private:
    KNMusicBackend *m_bassBackend;
    QSignalMapper *m_tuneRelease, *m_textChange;
    KNLabelEditor *m_equalizerEdit[KNMusicBackend::EqualizerCount],
                  *m_rangeEdit;
    KNMusicEQSlider *m_equalizerTune[KNMusicBackend::EqualizerCount],
                    *m_rangeControl;
    QLabel *m_equalizerCaption[KNMusicBackend::EqualizerCount],
           *m_rangeLabel;
    QString m_rangeCaption;
};

#endif // KNMUSICEQ_H
