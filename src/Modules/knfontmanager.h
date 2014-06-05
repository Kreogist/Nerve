#ifndef KNFONTMANAGER_H
#define KNFONTMANAGER_H

#include <QObject>

class KNFontManager : public QObject
{
    Q_OBJECT
public:
    static KNFontManager *instance();
    void loadCustomFont();
    void setGlobalFont(const QString &familyName);

signals:

public slots:

private:
    KNFontManager();
    static KNFontManager *m_instance;
    int m_fontSize=14;
};

#endif // KNFONTMANAGER_H
