#ifndef KNMUSICLISTVIEWHEADER_H
#define KNMUSICLISTVIEWHEADER_H

#include "knmusiclistviewheaderbase.h"

class KNMusicListViewHeaderMenu;
class KNMusicListViewHeader : public KNMusicListViewHeaderBase
{
    Q_OBJECT
public:
    explicit KNMusicListViewHeader(QWidget *parent = 0);

private slots:
    void showContextMenu(const QPoint &mousePoint);

private:
    KNMusicListViewHeaderMenu *m_viewerMenu;
};

#endif // KNMUSICLISTVIEWHEADER_H
