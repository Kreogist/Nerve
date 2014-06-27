#ifndef KNLIBSEARCHER_H
#define KNLIBSEARCHER_H

#include <QList>
#include <QUrl>
#include <QFileInfo>
#include <QStringList>

#include <QObject>

class KNLibSearcher : public QObject
{
    Q_OBJECT
public:
    explicit KNLibSearcher(QObject *parent = 0);

signals:
    void requireAddRawFiles(QStringList fileList);

public slots:
    void analysisUrls(const QList<QUrl> &urls);

protected:
    virtual int getType(const QString &suffix);

private:
    void analysisFolder(const QString &folderPath);
    void analysisFile(const QString &suffix,
                      const QString &filePath);
    QFileInfo m_currentDetails;
    QStringList m_rawFileList;
};

#endif // KNLIBSEARCHER_H
