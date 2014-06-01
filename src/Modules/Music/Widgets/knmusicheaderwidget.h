#ifndef KNMUSICHEADERWIDGET_H
#define KNMUSICHEADERWIDGET_H

#include <QModelIndex>

#include "../../Base/knstdlibheaderwidget.h"

class QBoxLayout;
class QResizeEvent;
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
    void requireShowInCurrent(const QModelIndex &index);
    void requireLostFocus();

public slots:
    void retranslate();
    void retranslateAndSet();
    void setSearchFocus();
    void clearSearch();
    void onActionPlayMusic(const QModelIndex &index);

protected:
    void resizeEvent(QResizeEvent *event);

private:
    QBoxLayout *m_mainLayout;
    QWidget *m_visualEffect;
    KNSearchBox *m_searchBox;
    KNMusicModel *m_musicModel;
    KNMusicHeaderPlayer *m_headerPlayer;
    QString m_searchPlaceHolder;
    QModelIndex m_currentIndex;
};

#endif // KNMUSICHEADERWIDGET_H
