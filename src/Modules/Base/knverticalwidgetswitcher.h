#ifndef KNWIDGETSWITCHER_H
#define KNWIDGETSWITCHER_H

#include <QList>
#include <QWidget>

class QPropertyAnimation;
class QParallelAnimationGroup;
class QResizeEvent;
class KNVerticalWidgetSwitcher : public QWidget
{
    Q_OBJECT
public:
    explicit KNVerticalWidgetSwitcher(QWidget *parent = 0);
    void addWidget(QWidget *widget);
    int currentIndex() const;
    void setWidgetFocus(const int &index);
    int size() const;

signals:
    void movedComplete();

public slots:
    void setCurrentIndex(int currentIndex);

protected:
    void resizeEvent(QResizeEvent *event);

private slots:
    void hideMovedOut();

private:
    void setWidgetVisible(const int &index, const bool &visible);

    QList<QWidget *> m_widgets;
    int m_currentIndex=-1, m_movedOutPage=-1;
    QParallelAnimationGroup *m_switchAnime;
    QPropertyAnimation *m_flyOutAnime, *m_flyInAnime;
};

#endif // KNWIDGETSWITCHER_H
