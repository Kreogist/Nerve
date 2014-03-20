#ifndef KNMUSICSEARCHER_H
#define KNMUSICSEARCHER_H

#include <QList>
#include <QUrl>
#include <QStringList>

#include <QObject>

class KNMusicSearcher : public QObject
{
    Q_OBJECT
public:
    explicit KNMusicSearcher(QObject *parent = 0);

signals:
    void requireAnalysis(const QString &filePath);

public slots:
    void analysisList(const QList<QUrl> &urls);

private:
    void searchDir();
    QStringList m_suffixs;
};

#endif // KNMUSICSEARCHER_H
