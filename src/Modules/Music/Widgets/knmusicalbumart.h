#ifndef KNMUSICALBUMART_H
#define KNMUSICALBUMART_H

#include <QLinearGradient>

#include <QLabel>

class QResizeEvent;
class QPaintEvent;
class KNMusicAlbumArt : public QLabel
{
    Q_OBJECT
public:
    explicit KNMusicAlbumArt(QWidget *parent = 0);

signals:
    void requireShowMusicPlayer();
    void requireHideMusicPlayer();

protected:
    void mousePressEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
    void paintEvent(QPaintEvent *event);
    void resizeEvent(QResizeEvent *event);

private:
    bool m_isPressed;
    bool m_isPlayerShown=false;
    QLinearGradient m_highLight;
    QPointF m_highLightArea[3];
};

#endif // KNMUSICALBUMART_H
