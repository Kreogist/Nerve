#ifndef KNLIBMEDIAINFO_H
#define KNLIBMEDIAINFO_H

#include <QObject>

class KNLibMediaInfo : public QObject
{
    Q_OBJECT
public:
    explicit KNLibMediaInfo(QObject *parent = 0);
    void analysisFile(const QString &filePath);
    QString originalData() const;

signals:

public slots:

private:
    QString m_originalData;
};

#endif // KNLIBMEDIAINFO_H
