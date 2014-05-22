#ifndef KNLIBMEDIAINFOPARSER_H
#define KNLIBMEDIAINFOPARSER_H

#include <QMap>
#include <QList>

#include <QObject>

class KNLibMediaInfoParser : public QObject
{
    Q_OBJECT
public:
    struct MediaInfoBlock
    {
        QString name;
        QList<QString> key;
        QList<QString> data;
    };
    explicit KNLibMediaInfoParser(QObject *parent = 0);
    void parseData(const QString &rawInfoData);
    int blockCount() const;
    MediaInfoBlock blockAt(const int &index) const;
    void setCurrentBlock(const QString &blockName);
    QString data(const QString &key) const;

signals:

public slots:

private:
    QList<MediaInfoBlock> m_data;
    int m_currentBlockIndex;
};

#endif // KNLIBMEDIAINFOPARSER_H
