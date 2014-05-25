#ifndef KNMUSICDETAILTOOLTIP_H
#define KNMUSICDETAILTOOLTIP_H

#include <QWidget>
#include <QModelIndex>

class QTimer;
class QLabel;
class QBoxLayout;
class KNMusicModel;
class KNMusicDetailTooltip : public QWidget
{
    Q_OBJECT
public:
    explicit KNMusicDetailTooltip(QWidget *parent = 0);
    void setTooltip(const QModelIndex &index,
                    const QPoint &point);
    void setMusicModel(KNMusicModel *model);
    void showTooltip();

signals:

public slots:
    void retranslate();
    void retranslateAndSet();

protected:
    void enterEvent(QEvent *event);
    void leaveEvent(QEvent *event);

private slots:
    void onActionHide();

private:
    enum ToolTipItems
    {
        Title,
        Time,
        Artist,
        ToolTipItemsCount
    };

    QPoint bestPosition(const QPoint &pos);
    QLabel *m_albumArt, *m_labels[ToolTipItemsCount];
    QString m_caption[ToolTipItemsCount];
    QTimer *m_tooltipDisapper;
    int m_currentRow;
    KNMusicModel *m_musicModel;
};

#endif // KNMUSICDETAILTOOLTIP_H
