#ifndef KNMUSICTAGID3V1_H
#define KNMUSICTAGID3V1_H

#include <QStringList>

#include "knmusictagbase.h"

class KNMusicTagID3v1 : public KNMusicTagBase
{
    Q_OBJECT
public:
    struct ID3v1Data
    {
        QString title;
        QString artist;
        QString album;
        QString year;
        QString comment;
        qint64 track;
        QString genre;
    };
    explicit KNMusicTagID3v1(QObject *parent = 0);
    bool readTag(const QString &filePath);

    KNMusicTagID3v1::ID3v1Data tagData() const;
    void setTagData(const ID3v1Data &tagData);

signals:

public slots:

private:
    QString getGenre(const int &index);
    ID3v1Data m_tagData;
    QStringList m_genreList;
};

#endif // KNMUSICTAGID3V1_H
