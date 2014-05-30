#ifndef KNMUSICVISUALEFFECT_H
#define KNMUSICVISUALEFFECT_H

#include <QWidget>

class KNMusicVisualEffect : public QWidget
{
    Q_OBJECT
public:
    explicit KNMusicVisualEffect(QWidget *parent = 0);
    float *fftData();

signals:

public slots:

protected:
    void paintEvent(QPaintEvent *event);

private:
    float m_fft[1024]={0};
};

#endif // KNMUSICVISUALEFFECT_H
