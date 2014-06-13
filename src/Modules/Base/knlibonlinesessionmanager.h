#ifndef KNLIBONLINESESSIONMANAGER_H
#define KNLIBONLINESESSIONMANAGER_H

#include <QVariant>
#include <QObject>

class KNLibOnlineSessionManager : public QObject
{
    Q_OBJECT
public:
    explicit KNLibOnlineSessionManager(QObject *parent = 0);
    struct NetworkReply
    {
        QVariant id;
        QByteArray data;
    };

signals:

public slots:

};

#endif // KNLIBONLINESESSIONMANAGER_H
