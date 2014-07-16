#ifndef KNSCROLLLABEL_H
#define KNSCROLLLABEL_H

#include <QWidget>
#include <QLabel>

class QTimer;
class KNScrollLabel : public QWidget
{
    Q_OBJECT
public:
    explicit KNScrollLabel(QWidget *parent = 0);
    QString text() const;
    void setText(const QString &text);
    void setFont(const QFont &font);

signals:

public slots:

protected:
    void paintEvent(QPaintEvent *event);
    void resizeEvent(QResizeEvent *event);

private slots:
    void onActionMoving();
    void onActionWatingFinished();

private:
    void updateAnimeParam();
    void stopAllTimer();
    QString m_text;
    QTimer *m_movingAnime, *m_waiting;
    int m_textX=0, m_leftMostX=0;
    bool m_movingLeft=true;
};

#endif // KNSCROLLLABEL_H
