#ifndef KNMUSICPLAYERWIDGET_H
#define KNMUSICPLAYERWIDGET_H

#include <QWidget>

class QAbstractItemModel;
class QPropertyAnimation;
class QLabel;
class QSpacerItem;
class QBoxLayout;
class KNMusicModel;
class KNOpacityButton;
class KNMusicBackend;
class KNMusicHeaderPlayer;
class KNMusicVisualEffect;
class KNMusicPlayerWidget : public QWidget
{
    Q_OBJECT
public:
    explicit KNMusicPlayerWidget(QWidget *parent = 0);
    void setEqualizer(QWidget *equalizer);
    void setBackend(KNMusicBackend *backend);
    void setHeaderPlayer(KNMusicHeaderPlayer *headerPlayer);
    void setArtwork(const QPixmap &artwork);

signals:

public slots:
    void setPlayListModel(QAbstractItemModel *model);
    void retranslate();
    void retranslateAndSet();
    void syncData();

protected:
    void resizeEvent(QResizeEvent *event);

private slots:
    void onActionPositionChanged(const int &position);
    void onActionShowEqualizer();
    void onActionHideEqualizer();

private:
    QLabel *m_albumArtWork, *m_nowPlaying, *m_artistAlbumName, *m_title;
    QFont m_nowPlayingFont, m_titleFont;
    QWidget *m_equalizer;
    QString m_nowPlayingText, m_noAlbumName, m_noArtistName;
    QPropertyAnimation *m_equalizerShow, *m_equalizerHide;
    QBoxLayout *m_mainLayout, *m_artworkLayout, *m_detailsLayout, *m_controlsLayout;
    KNMusicHeaderPlayer *m_headerPlayer;
    KNMusicBackend *m_backend;
    KNMusicVisualEffect *m_visualEffect;
    KNOpacityButton *m_equalizerControl, *m_playlistControl;
};

#endif // KNMUSICPLAYERWIDGET_H
