#ifndef KNMUSICHEADERWIDGET_H
#define KNMUSICHEADERWIDGET_H

#include "../../Base/knstdlibheaderwidget.h"

class QBoxLayout;
class KNMusicModel;
class KNMusicPlayer;
class KNSearchBox;
class KNMusicHeaderPlayer;
class KNMusicHeaderWidget : public KNStdLibHeaderWidget
{
    Q_OBJECT
public:
    explicit KNMusicHeaderWidget(QWidget *parent = 0);
    void setMusicModel(KNMusicModel *model);
    void setMusicPlayer(KNMusicPlayer *player);

signals:
    void requireSearch(const QString &text);
    void requireLostFocus();

public slots:
    void retranslate();
    void retranslateAndSet();
    void setSearchFocus();
    void clearSearch();
    void onActionPlayMusic(const QModelIndex &index);

private:
    QBoxLayout *m_mainLayout;
    KNSearchBox *m_searchBox;
    KNMusicModel *m_musicModel;
    KNMusicHeaderPlayer *m_headerPlayer;
    QString m_searchPlaceHolder;
};

#endif // KNMUSICHEADERWIDGET_H
