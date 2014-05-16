#ifndef KNMUSICARTISTITEM_H
#define KNMUSICARTISTITEM_H

#include <QStandardItem>

class KNMusicArtistItem : public QStandardItem
{
public:
    explicit KNMusicArtistItem(QString name = "");
    bool hasIcon() const;
    void setHasIcon(const bool &value);

    QString iconKey() const;
    void setIconKey(const QString &iconKey);

signals:

public slots:

private:
    bool m_hasIcon=false;
    QString m_iconKey;
};

#endif // KNMUSICARTISTITEM_H
