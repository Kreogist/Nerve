#ifndef KNMUSICDETAILTOOLTIP_H
#define KNMUSICDETAILTOOLTIP_H

#include <QWidget>
#include <QModelIndex>
#include <QLabel>

class QHideEvent;
class QLabel;
class QKeyEvent;
class QTimer;
class QTimeLine;
class QToolButton;
class QSlider;
class QMouseEvent;
class QBoxLayout;
class KNMusicLibraryModelBase;
class KNPlayerProgress;
class KNMusicBackend;
class KNMusicDetailTooltipPlay : public QLabel
{
    Q_OBJECT
public:
    explicit KNMusicDetailTooltipPlay(QWidget *parent = 0);
    void reset();
    bool isPaused() const;
    void setState(const bool &shownPlay);

signals:
    void requirePause();
    void requirePlay();

protected:
    void mouseReleaseEvent(QMouseEvent *event);

private:
    QPixmap m_play, m_pause;
    bool m_isButtonPlay=true;
};

class KNMusicDetailTooltip : public QWidget
{
    Q_OBJECT
public:
    explicit KNMusicDetailTooltip(QWidget *parent = 0);
    ~KNMusicDetailTooltip();
    void setTooltip(const QModelIndex &index,
                    const QPoint &point);
    void setMusicModel(KNMusicLibraryModelBase *model);
    void showTooltip();
    void forceQuit();
    void setMusicBackend(KNMusicBackend *backend);
    void resetPlayStatus();

signals:

public slots:
    void retranslate();
    void retranslateAndSet();

protected:
    void enterEvent(QEvent *event);
    void leaveEvent(QEvent *event);
    void hideEvent(QHideEvent *event);
    void keyPressEvent(QKeyEvent *event);

private slots:
    void onActionHide();
    void onActionPositionChanged(qint64 test);
    void onActionSliderReleased();
    void onActionSliderPressed();
    void onActionChangeBackground(const int &colorParam);
    void onActionPreviewFinished();

private:
    enum ToolTipItems
    {
        Title,
        FileName,
        Time,
        Artist,
        FilePath,
        ToolTipItemsCount
    };
    void setEliedText(QLabel *widget, const QString &rawTextData);
    QPoint bestPosition(const QPoint &pos);
    QLabel *m_albumArt, *m_labels[ToolTipItemsCount];
    QString m_inFile, m_filePath;
    QBoxLayout *m_mainLayout;
    QTimer *m_tooltipDisapper;
    int m_currentRow=-1, m_fixedWidth=448, m_fixedHeight=176;
    bool m_sliderPressed=false;
    KNMusicLibraryModelBase *m_musicModel;
    KNMusicBackend *m_preview;
    KNPlayerProgress *m_playerStatus;
    KNMusicDetailTooltipPlay *m_control;
    QTimeLine *m_mouseIn, *m_mouseOut;
    QWidget *m_labelContainer;
    QBoxLayout *m_labelLayout;
    QColor m_background;
    QPalette m_palette;
};

#endif // KNMUSICDETAILTOOLTIP_H
