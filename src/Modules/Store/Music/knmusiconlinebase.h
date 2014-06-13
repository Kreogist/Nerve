#ifndef KNMUSICONLINEBASE_H
#define KNMUSICONLINEBASE_H

#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QNetworkCookie>
#include <QScopedPointer>
#include <QUrl>
#include <QThread>
#include <QByteArray>
#include <QMap>

#include <QObject>

class KNMusicOnlineBase : public QObject
{
    Q_OBJECT
public:
    explicit KNMusicOnlineBase(QObject *parent = 0);

signals:
    void modelUpdate();

public slots:

};

#endif // KNMUSICONLINEBASE_H
