#ifndef KNMUSICPLAYLISTLISTEDITOR_H
#define KNMUSICPLAYLISTLISTEDITOR_H

#include <QLinearGradient>

#include "../Base/knmusicplaylistlisteditorbase.h"

class QAction;
class QResizeEvent;
class QTimeLine;
class KNMenu;
class KNOpacityButton;
class KNMusicPlaylistListEditor : public KNMusicPlaylistListEditorBase
{
    Q_OBJECT
public:
    explicit KNMusicPlaylistListEditor(QWidget *parent = 0);

signals:

public slots:
    void retranslate();
    void retranslateAndSet();

protected:
    void enterEvent(QEvent *event);
    void leaveEvent(QEvent *event);
    void resizeEvent(QResizeEvent *event);

private slots:
    void changeBackground(const int &frame);
    void showAddMenu();

private:
    void createAddMenu();
    void createRemove();
    enum AddCategory
    {
        Playlist,
        CategoryCount
    };
    QString m_addActionCaption[CategoryCount];
    QAction *m_addAction[CategoryCount];
    KNMenu *m_addMenu;
    KNOpacityButton *m_add, *m_remove, *m_edit;

    //UI tweaks.
    QPalette m_palette;
    QLinearGradient m_background;
    QColor m_backgroundColor;
    QTimeLine *m_mouseIn, *m_mouseOut;
};

#endif // KNMUSICPLAYLISTLISTEDITOR_H
