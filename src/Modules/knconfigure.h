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
    QVariant value(const QString &key);
    void setGroup(const QString &groupName);

signals:

public slots:

private:
    static KNConfigure *m_instance;
    KNConfigure();
    QString m_configurePath;
    QSettings *m_settings;
};

#endif // KNCONFIGURE_H
