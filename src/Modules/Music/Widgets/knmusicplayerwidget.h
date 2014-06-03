#ifndef KNMUSICPLAYERWIDGET_H
#define KNMUSICPLAYERWIDGET_H

#include <QWidget>
#include <QAbstractItemView>

class QLabel;
class QSpacerItem;
class QBoxLayout;
class KNMusicModel;
class KNMusicBackend;
class KNMusicHeaderWidget;
class KNMusicVisualEffect;
class KNMusicPlayerWidget : public QWidget
{
    Q_OBJECT
public:
    explicit KNMusicPlayerWidget(QWidget *parent = 0);
    void setEqualizer(QWidget *equalizer);
    void setBackend(KNMusicBackend *backend);
    void setMusicModel(KNMusicModel *model);
    void setArtwork(const QPixmap &artwork);

signals:

public slots:
    void retranslate();
    void retranslateAndSet();
    void syncData(const QModelIndex &index);

protected:
    void resizeEvent(QResizeEvent *event);

private slots:
    void onActionPositionChanged(const int &position);

private:
    QLabel *m_albumArtWork, *m_nowPlaying, *m_artistAlbumName, *m_title;
    QFont m_nowPlayingFont, m_titleFont;
    QWidget *m_equalizer;
    QString m_nowPlayingText, m_noAlbumName, m_noArtistName;
    QBoxLayout *m_mainLayout, *m_playListLayout, *m_artworkLayout;
    KNMusicHeaderWidget *m_headerPlayer;
    KNMusicModel *m_musicModel;
    QString m_albumName, m_artistName;
    bool m_noAlbum, m_noAritst;
    KNMusicBackend *m_backend;
    KNMusicVisualEffect *m_visualEffect;
};

#endif // KNMUSICPLAYERWIDGET_H
