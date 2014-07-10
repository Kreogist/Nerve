#ifndef KNMUSICLRCPARSERBASE_H
#define KNMUSICLRCPARSERBASE_H

#include <QObject>

class KNMusicLRCParserBase : public QObject
{
    Q_OBJECT
public:
    explicit KNMusicLRCParserBase(QObject *parent = 0);
    virtual bool readLyrics(QString filePath)=0;
    virtual QString title() const=0;
    virtual QString artist() const=0;
    virtual QString album() const=0;

signals:

public slots:

};

#endif // KNMUSICLRCPARSERBASE_H
