#ifndef KNMUSICPLAYLISTLISTEDITOR_H
#define KNMUSICPLAYLISTLISTEDITOR_H

#include <QWidget>

class KNOpacityButton;
class KNMusicPlaylistListEditor : public QWidget
{
    Q_OBJECT
public:
    explicit KNMusicPlaylistListEditor(QWidget *parent = 0);

signals:

public slots:

private:
    KNOpacityButton *m_add, *m_remove, *m_edit;
};

#endif // KNMUSICPLAYLISTLISTEDITOR_H
