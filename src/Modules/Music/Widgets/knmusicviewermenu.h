#ifndef KNMUSICVIEWERMENU_H
#define KNMUSICVIEWERMENU_H

#include "../../../Modules/Base/knmenu.h"

class KNGlobal;
class QStandardItem;
class KNMusicViewerMenu : public KNMenu
{
    Q_OBJECT
public:
    explicit KNMusicViewerMenu(QWidget *parent = 0);
    void setFilePath(const QString &filePath);
    void setItem(const QStandardItem *item);

signals:

public slots:
    void retranslate();
    void retranslateAndSet();

private slots:
    void onActionCopy();
    void onActionPlay();
    void onActionCopyText();
    void onActionBrowse();

private:
    enum MusicActions
    {
        Play,
        Browse,
        Copy,
        CopyText,
        MusicActionCount
    };
    QString m_actionTitle[MusicActionCount];
    QAction *m_action[MusicActionCount];
    void createActions();

    QString m_filePath, m_itemText;
    KNGlobal *m_global;
};

#endif // KNMUSICVIEWERMENU_H
