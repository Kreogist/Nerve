#ifndef KNMUSICARTISTVIEW_H
#define KNMUSICARTISTVIEW_H

#include <QSplitter>

class QListView;
class QLabel;
class QAbstractItemModel;
class KNMusicArtistDetailsDisplay : public QWidget
{
    Q_OBJECT
public:
    explicit KNMusicArtistDetailsDisplay(QWidget *parent = 0);

private:
    QLabel *m_artistName, *m_artistInfo;
};

class KNMusicArtistView : public QSplitter
{
    Q_OBJECT
public:
    explicit KNMusicArtistView(QWidget *parent = 0);
    void setModel(QAbstractItemModel *model);

signals:

public slots:
    void resort();

private:
    QListView *m_artistList;
    KNMusicArtistDetailsDisplay *m_artistDetails;
};

#endif // KNMUSICARTISTVIEW_H
