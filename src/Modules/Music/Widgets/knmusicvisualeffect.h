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

signals:

public slots:

protected:
    void paintEvent(QPaintEvent *event);

private:
    float qsqrt(const float &number);
    float m_fft[2048]={0};
    QColor m_itemColor=QColor(255,255,255);
};

#endif // KNMUSICVISUALEFFECT_H
