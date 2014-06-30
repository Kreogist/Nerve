#ifndef KNMUSICPLAYLISTLISTEDITOR_H
#define KNMUSICPLAYLISTLISTEDITOR_H

#include <QLinearGradient>

#include "../Base/knmusicplaylistlisteditorbase.h"

class QTimeLine;
class KNOpacityButton;
class KNMusicPlaylistListEditor : public KNMusicPlaylistListEditorBase
{
    Q_OBJECT
public:
    explicit KNMusicPlaylistListEditor(QWidget *parent = 0);

signals:

public slots:

protected:
    void enterEvent(QEvent *event);
    void leaveEvent(QEvent *event);

private slots:
    void changeBackground(const int &frame);

private:
    KNOpacityButton *m_add, *m_remove, *m_edit;
    QPalette m_palette;
    QLinearGradient m_background;
    QColor m_backgroundColor;
    QTimeLine *m_mouseIn, *m_mouseOut;
};

#endif // KNMUSICPLAYLISTLISTEDITOR_H
