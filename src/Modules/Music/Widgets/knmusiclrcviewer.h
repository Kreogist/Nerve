#ifndef KNMUSICLRCVIEWER_H
#define KNMUSICLRCVIEWER_H

#include <QList>
#include <QWidget>

class QTimeLine;
class KNMusicBackend;
class KNMusicLRCParser;
class KNMusicLRCViewer : public QWidget
{
    Q_OBJECT
public:
    explicit KNMusicLRCViewer(QWidget *parent = 0);
    void loadLyrics(const QString &filePath);
    void setMusicBackend(KNMusicBackend *player);

signals:

public slots:
    void onActionPositionChanged(const float &position);

protected:
    void paintEvent(QPaintEvent *event);
    void resizeEvent(QResizeEvent *event);

private:
    void resetViewer();
    KNMusicLRCParser *m_lrcParser;
    QList<float> m_positions;
    QStringList m_lyrics;
    int m_currentLine, m_nextPosition, m_centerY, m_paintingY;
    QTimeLine *m_centerAnime;
};

#endif // KNMUSICLRCVIEWER_H
