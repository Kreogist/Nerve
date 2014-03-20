#ifndef KNMUSICVIEWERMENU_H
#define KNMUSICVIEWERMENU_H

#include "../../../Modules/Base/knmenu.h"

class KNMusicViewerMenu : public KNMenu
{
    Q_OBJECT
public:
    explicit KNMusicViewerMenu(QWidget *parent = 0);

signals:

public slots:
    void retranslate();
    void retranslateAndSet();

private:
    enum MusicActions
    {
        Browse,
        Copy,
        MusicActionCount
    };
    QString m_actionTitle[MusicActionCount];
    QAction *m_action[MusicActionCount];
    void createActions();
};

#endif // KNMUSICVIEWERMENU_H
