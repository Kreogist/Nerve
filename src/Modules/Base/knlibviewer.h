#ifndef KNLIBVIEWER_H
#define KNLIBVIEWER_H

#include <QUrl>
#include <QList>
#include <QWidget>

class KNLibViewer : public QWidget
{
    Q_OBJECT
public:
    explicit KNLibViewer(QWidget *parent = 0);

signals:
    void requireAnalysisUrls(QList<QUrl> urls);

public slots:

private:

};

#endif // KNLIBVIEWER_H
