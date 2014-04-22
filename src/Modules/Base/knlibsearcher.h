#ifndef KNLIBSEARCHER_H
#define KNLIBSEARCHER_H

#include <QList>
#include <QUrl>
#include <QObject>

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
    KNModel *m_model;
};

#endif // KNLIBSEARCHER_H
