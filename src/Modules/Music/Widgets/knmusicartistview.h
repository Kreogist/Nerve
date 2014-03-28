#ifndef KNMUSICARTISTVIEW_H
#define KNMUSICARTISTVIEW_H

#include <QTreeView>
#include <QListView>
#include <QSplitter>
#include <QThread>
#include <QScopedPointer>

#include "knmusiclistview.h"

class QLabel;
class QTimeLine;
class QStandardItemModel;
class KNMusicArtistModel;
class KNMusicArtistDetailModel;

class KNMusicArtistSongs : public KNMusicListView
{
    Q_OBJECT
public:
    explicit KNMusicArtistSongs(QWidget *parent = 0);
    void resetHeader();

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
    void resetHeader();

signals:
    void requireOpenUrl(const QModelIndex &index);

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
    void onVerticalScrollValueChange(int value);

private:
    QTimeLine *m_mouseIn, *m_mouseOut;
    QPalette m_palette;
    QColor m_backgroundColor;
    int m_minGrey=0x30;
};

class KNMusicArtistView : public QSplitter
{
    Q_OBJECT
public:
    explicit KNMusicArtistView(QWidget *parent = 0);
    void resetHeader();
    void setModel(KNMusicArtistModel *model);
    void setDetailModel(KNMusicArtistDetailModel *model);

signals:
    void requireDisplayDetails(const QModelIndex &current);
    void requireOpenUrl(const QModelIndex &index);

public slots:
    void resort();
    void onActionDetailCountChange(const int &value);

private slots:
    void onActionItemActivate(const QModelIndex & current,
                              const QModelIndex & previous);

private:
    KNMusicArtistList *m_artistList;
    KNMusicArtistDetailsDisplay *m_artistDetails;
    KNMusicArtistModel *m_artistModel;
};

#endif // KNMUSICARTISTVIEW_H
