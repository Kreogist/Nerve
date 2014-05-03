#ifndef KNLIBSEARCHER_H
#define KNLIBSEARCHER_H

#include <QList>
#include <QUrl>
#include <QObject>
#include <QFileInfo>

class KNModel;
class KNLibSearcher : public QObject
{
    Q_OBJECT
public:
    explicit KNLibSearcher(QObject *parent = 0);
    KNModel *model() const;
    void setModel(KNModel *model);

signals:
    void requireAnalysis(const QString &filePath);

public slots:
    void analysisList(const QList<QUrl> &urls);

protected:
    virtual int getType(const QString &suffix);

private:
    void analysisFolder(const QString &folderPath);
    void analysisFile(const QString &suffix,
                      const QString &filePath);
    QFileInfo m_currentDetails;
    KNModel *m_model;
};

#endif // KNLIBSEARCHER_H
