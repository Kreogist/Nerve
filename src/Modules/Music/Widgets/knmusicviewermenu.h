#ifndef KNMUSICVIEWERMENU_H
#define KNMUSICVIEWERMENU_H

#include "../../../Modules/Base/knmenu.h"

class KNGlobal;
class KNMusicViewerMenu : public KNMenu
{
    Q_OBJECT
public:
    explicit KNMusicViewerMenu(QWidget *parent = 0);
    void setFilePath(const QString &filePath);

signals:

public slots:
    void retranslate();
    void retranslateAndSet();

private slots:
    void onActionCopy();
    void onActionPlay();
    void onActionBrowse();

private:
    enum MusicActions
    {
        Play,
        Browse,
        Copy,
        MusicActionCount
    };
    QString m_actionTitle[MusicActionCount];
    QAction *m_action[MusicActionCount];
    void createActions();

    QString m_filePath;
    KNGlobal *m_global;
};

#endif // KNMUSICVIEWERMENU_H
