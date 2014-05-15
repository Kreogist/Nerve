#ifndef KNLIBDATABASE_H
#define KNLIBDATABASE_H

#include <QFileInfo>
#include <QJsonObject>
#include <QJsonDocument>
#include <QJsonArray>
#include <QStringList>
#include <QObject>

class QFile;
class KNLibDatabase : public QObject
{
    Q_OBJECT
public:
    explicit KNLibDatabase(QObject *parent = 0);
    QJsonValue at(int i) const;
    QJsonObject row(int i) const;
    void setDatabase(const QString &filePath);
    virtual void load();
    void replace(int i, const QJsonValue &value);
    void setBatchCount(int batch);
    void append(const QJsonValue &value);
    QJsonValue takeAt(int i);
    void removeAt(int i);
    int size() const;
    void flush();

signals:

public slots:

private:
    bool readFromDisk();
    void writeToDisk();
    void clearCache();
    void createDatabase();
    void reduceBatchCount();
    QString m_databaseFilePath;
    QFile *m_databaseFile;
    QFileInfo m_databaseFileInfo;
    QJsonObject m_content;
    QJsonDocument m_document;
    QJsonArray m_databaseArray;
    int m_majorVersion=0, m_minorVersion=1, m_batchCount=10,
        m_currentBatchCount=10;
    bool m_databaseChanged=false;
};

#endif // KNLIBDATABASE_H
