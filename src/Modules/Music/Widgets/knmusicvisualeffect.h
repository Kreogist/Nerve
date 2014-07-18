#ifndef KNMUSICVISUALEFFECT_H
#define KNMUSICVISUALEFFECT_H

#include <QPixmap>
#include <QWidget>

class KNMusicVisualEffect : public QWidget
{
    Q_OBJECT
public:
    explicit KNMusicVisualEffect(QWidget *parent = 0);
    float *fftData();
    void setSampleRate(float sampleRate);
    void prepareGraphic();

signals:

public slots:

protected:
    void paintEvent(QPaintEvent *event);

private:
    float m_fft[2047]={0},
          m_topPoint[20]={50,     69,     94,     129,    176,
                          241,    331,    453,    620,    850,
                          1200,   1600,   2200,   3000,   4100,
                          5600,   7700,   11000,  14000,  20000
                         },
          m_frameHeight[20];
    int m_framePoint[20], m_maxPoint[20];
    QColor m_itemColor=QColor(255,255,255);
};

#endif // KNMUSICVISUALEFFECT_H
