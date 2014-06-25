#ifndef KNMUSICRATINGEDITOR_H
#define KNMUSICRATINGEDITOR_H

#include <QPixmap>
#include <QWidget>

class KNMusicRatingEditor : public QWidget
{
    Q_OBJECT
public:
    explicit KNMusicRatingEditor(QWidget *parent = 0);
    QSize sizeHint() const
    {
        return QSize(m_editorWidth, m_starSizeHint);
    }

    int starSizeHint() const;
    void setStarSizeHint(int starSizeHint);
    int starNum() const;
    void setStarNum(int starNum);

signals:
    void editingFinished();

protected:
    void paintEvent(QPaintEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);

private:
    int starAtPosition(int x);
    int m_starSizeHint=0, m_editorWidth=0, m_starNum, m_halfStarSizeHint;
    QPixmap m_star, m_unstar, m_scaleStar, m_scaleUnstar;
};

#endif // KNMUSICRATINGEDITOR_H
