#ifndef KNMUSICVIEWCONTAINER_H
#define KNMUSICVIEWCONTAINER_H

#include <QUrl>
#include <QList>
#include <QWidget>

class QBoxLayout;
class KNMusicViewContainer : public QWidget
{
    Q_OBJECT
public:
    explicit KNMusicViewContainer(QWidget *parent = 0);
    void setCentralWidget(QWidget *widget);

signals:
    void requireAnalysisUrls(QList<QUrl> urls);
    void dragEntered();
    void dropped();

public slots:

protected:
    void dragEnterEvent(QDragEnterEvent *event);
    void dropEvent(QDropEvent *event);

private:
    QBoxLayout *m_layout;
};

#endif // KNMUSICVIEWCONTAINER_H
