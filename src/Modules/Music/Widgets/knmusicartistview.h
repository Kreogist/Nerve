#ifndef KNMUSICARTISTVIEW_H
#define KNMUSICARTISTVIEW_H

#include <QTreeView>
#include <QListView>
#include <QSplitter>
#include <QThread>
#include <QScopedPointer>

class QLabel;
class QTimeLine;
class QStandardItemModel;
class KNMusicArtistModel;
class KNMusicArtistDetailModel;

class KNMusicArtistSongs : public QTreeView
{
    Q_OBJECT
public:
    explicit KNMusicArtistSongs(QWidget *parent = 0);

private:

};

class KNMusicArtistDetailsDisplay : public QWidget
{
    Q_OBJECT
public:
    explicit KNMusicArtistDetailsDisplay(QWidget *parent = 0);
    void setArtistName(const QString &artistName);
    void setSongNumber(const int &index);
    void setDetailModel(KNMusicArtistDetailModel *model);

private:
    QLabel *m_artistName, *m_artistInfo;
    QString m_song, m_songs;
    KNMusicArtistSongs *m_songViewer;
};

class KNMusicArtistList : public QListView
{
    Q_OBJECT
public:
    explicit KNMusicArtistList(QWidget *parent = 0);

protected:
    void enterEvent(QEvent *e);
    void leaveEvent(QEvent *e);

private slots:
    void changeBackground(int frameData);

private:
    QTimeLine *m_mouseIn, *m_mouseOut;
    QPalette m_palette;
    QColor m_backgroundColor;
    int m_minGrey=0x20;
};

class KNMusicArtistView : public QSplitter
{
    Q_OBJECT
public:
    explicit KNMusicArtistView(QWidget *parent = 0);
    void setModel(KNMusicArtistModel *model);
    void setDetailModel(KNMusicArtistDetailModel *model);

signals:
    void requireDisplayDetails(const QModelIndex &current);

public slots:
    void resort();

private slots:
    void onActionItemActivate(const QModelIndex & current,
                              const QModelIndex & previous);

private:
    KNMusicArtistList *m_artistList;
    KNMusicArtistDetailsDisplay *m_artistDetails;
    KNMusicArtistModel *m_artistModel;
};

#endif // KNMUSICARTISTVIEW_H
