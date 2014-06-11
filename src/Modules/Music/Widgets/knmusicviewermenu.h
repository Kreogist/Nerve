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
    void setMode(KNMusicGlobal::MusicCategory category);
    void readIndexesFromGlobal();
    void setItemIndex(const QModelIndex &index);
    void setModel(QStandardItemModel *model);

signals:
    void requirePlayMusic(const QModelIndex &index);
    void requireShowIn(KNMusicGlobal::MusicCategory category,
                       const QModelIndex &index);
    void requireGetInfo(const QString &filePath);
    void requireDelete(const QModelIndex &index);
    void requireDeleteSelection();

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
    void onActionGetInfo();
    void onActionDelete();

private:
    enum MusicActions
    {
        Play,
        ShowInSongs,
        ShowInArtist,
        ShowInAlbum,
        ShowInGenre,
        GetInfo,
        Browse,
        Copy,
        CopyText,
        Delete,
        MusicActionCount
    };
    enum MusicBatchActions
    {
        DeleteSelects,
        MusicBatchActionCount
    };

    QString m_actionTitle[MusicActionCount],
            m_batchActionTitle[MusicBatchActionCount];
    QAction *m_action[MusicActionCount],
            *m_batchAction[MusicBatchActionCount];
    void createActions();

    QStandardItemModel *m_model;
    QModelIndex m_currentIndex;
    QString m_filePath, m_itemText;
    KNMusicGlobal *m_musicGlobal;
    KNGlobal *m_global;
};

#endif // KNMUSICVIEWERMENU_H
