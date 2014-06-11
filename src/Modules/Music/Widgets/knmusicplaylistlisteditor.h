#ifndef KNMUSICPLAYLISTLISTEDITOR_H
#define KNMUSICPLAYLISTLISTEDITOR_H

#include <QWidget>

class QBoxLayout;
class KNMenu;
class KNOpacityButton;
class KNMusicPlaylistListEditor : public QWidget
{
    Q_OBJECT
public:
    explicit KNMusicPlaylistListEditor(QWidget *parent = 0);

signals:
    void requireCreatePlaylist();
    void requireRemoveCurrent();

public slots:
    void retranslate();
    void retranslateAndSet();

private:
    enum ActionsAdd
    {
        AddPlaylist,
        ActionAddCount
    };
    void createActions();
    QBoxLayout *m_toolsLayout;
    KNOpacityButton *m_createList, *m_removeList;
    KNMenu *m_itemAdd;
    QAction *m_itemAddAction[ActionAddCount];
    QString m_itemAddCaption[ActionAddCount];
};

#endif // KNMUSICPLAYLISTLISTEDITOR_H
