#ifndef KNLOCALE_H
#define KNLOCALE_H

#include <QObject>

class KNLocale : public QObject
{
    Q_OBJECT
public:
    static KNLocale *instance();

signals:
    void languageChanged();

public slots:

private:
    static KNLocale *m_instance;
    KNLocale();
};

#endif // KNLOCALE_H
