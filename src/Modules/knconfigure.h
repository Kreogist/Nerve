#ifndef KNCONFIGURE_H
#define KNCONFIGURE_H

#include <QObject>

class QSettings;
class KNConfigure : public QObject
{
    Q_OBJECT
public:
    static KNConfigure *instance();
    void setValue(const QString &key,
                  const QVariant &value);
    void setValue(const QString &groupName,
                  const QString &key,
                  const QVariant &value);
    QVariant value(const QString &key);
    QVariant value(const QString &groupName,
                   const QString &key);
    void beginGroup(const QString &groupName);
    void endGroup();

signals:

public slots:

private:
    static KNConfigure *m_instance;
    KNConfigure();
    QString m_configurePath;
    QSettings *m_settings;
};

#endif // KNCONFIGURE_H
