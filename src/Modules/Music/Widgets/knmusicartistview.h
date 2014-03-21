#ifndef KNMUSICARTISTVIEW_H
#define KNMUSICARTISTVIEW_H

#include <QWidget>

class QListView;
class QAbstractItemModel;
class KNMusicArtistView : public QWidget
{
    Q_OBJECT
public:
    explicit KNMusicArtistView(QWidget *parent = 0);
    void setModel(QAbstractItemModel *model);

signals:

public slots:

private:
    QListView *m_artistList;
};

#endif // KNMUSICARTISTVIEW_H
