#ifndef KNMUSICLISTVIEWHEADERMENU_H
#define KNMUSICLISTVIEWHEADERMENU_H

#include "../../Base/knmenu.h"

#include "../knmusicglobal.h"

class QAction;
class QSignalMapper;
class KNMusicListViewHeaderMenu : public KNMenu
{
    Q_OBJECT
public:
    explicit KNMusicListViewHeaderMenu(QWidget *parent = 0);
    void setHeaderValue(const int &index,
                        const bool &visible);

signals:
    void requireChangeVisible(const int &index,
                              const bool &visible);

public slots:
    void retranslate();
    void retranslateAndSet();

private slots:
    void onActionVisibleChanged(const int &index);

private:
    QAction *m_headerVisible[KNMusicGlobal::MusicDataCount];
    KNMusicGlobal *m_musicGlobal;
    QSignalMapper *m_headerVisibleChange;
};

#endif // KNMUSICLISTVIEWHEADERMENU_H
