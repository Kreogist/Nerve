#ifndef KNMUSICVIEWERMENU_H
#define KNMUSICVIEWERMENU_H

#include <QModelIndex>

#include "../knmusicglobal.h"

#include "../../../Modules/Base/knmenu.h"

class KNGlobal;
class QStandardItem;
class QStandardItemModel;
class KNMusicViewerMenu : public KNMenu
{
    Q_OBJECT
public:
    explicit KNMusicViewerMenu(QWidget *parent = 0);
    void setFilePath(const QString &filePath);
    void setItemIndex(const QModelIndex &index);
    void setModel(QStandardItemModel *model);
    void setMode(KNMusicGlobal::MusicCategory category);

signals:
    void requireShowIn(KNMusicGlobal::MusicCategory category,
                       const QModelIndex &index);

public slots:
    void retranslate();
    void retranslateAndSet();

private slots:
    void onActionCopy();
    void onActionPlay();
    void onActionCopyText();
    void onActionBrowse();
    void onActionShowInSongs();
    void onActionShowInArtist();
    void onActionShowInAlbum();
    void onActionShowInGenre();

private:
    enum MusicActions
    {
        Play,
        ShowInSongs,
        ShowInArtist,
        ShowInAlbum,
        ShowInGenre,
        Browse,
        Copy,
        CopyText,
        MusicActionCount
    };
    QString m_actionTitle[MusicActionCount];
    QAction *m_action[MusicActionCount];
    void createActions();

    QStandardItemModel *m_model;
    QModelIndex m_currentIndex;
    QString m_filePath, m_itemText;
    KNGlobal *m_global;
};

#endif // KNMUSICVIEWERMENU_H
