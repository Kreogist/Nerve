#ifndef KNMUSICPLAYERWIDGET_H
#define KNMUSICPLAYERWIDGET_H

#include <QWidget>

class QBoxLayout;
class KNMusicVisualEffect;
class KNMusicPlayerWidget : public QWidget
{
    Q_OBJECT
public:
    explicit KNMusicPlayerWidget(QWidget *parent = 0);
    ;

signals:

public slots:

private:
//    KNMusicVisualEffect *m_visualEffect;
    QWidget *m_equalizer;
    QBoxLayout *m_mainLayout, *m_playListLayout;
};

#endif // KNMUSICPLAYERWIDGET_H
