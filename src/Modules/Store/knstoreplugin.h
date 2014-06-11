#ifndef KNSTOREPLUGIN_H
#define KNSTOREPLUGIN_H

#include "../Base/knpluginbase.h"

#include <QWidget>

class KNStoreViewer;
class KNStorePlugin : public KNPluginBase
{
    Q_OBJECT
public:
    explicit KNStorePlugin(QObject *parent = 0);
    void applyPlugin();

signals:

public slots:

private:
    QWidget *text, *test;
    KNStoreViewer *m_storeViewer;
};

#endif // KNSTOREPLUGIN_H
