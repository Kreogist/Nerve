#ifndef KNMUSICARTISTITEM_H
#define KNMUSICARTISTITEM_H

#include <QStandardItem>

class KNMusicArtistItem : public QStandardItem
{
public:
    explicit KNMusicArtistItem(QString name = "");
    bool hasIcon() const;
    void setHasIcon(const bool &value);

signals:

public slots:

private:
    bool m_hasIcon=false;
};

#endif // KNMUSICARTISTITEM_H
