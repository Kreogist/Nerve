#ifndef KNMUSICRESULTVIEW_H
#define KNMUSICRESULTVIEW_H

#include <QWidget>
#include <QTreeView>
#include <QStyledItemDelegate>

class KNMusicResultItemDelegate : public QStyledItemDelegate
{
    Q_OBJECT
public:
    explicit KNMusicResultItemDelegate(QObject *parent = 0);

protected:
    void paint(QPainter *painter,
               const QStyleOptionViewItem &option,
               const QModelIndex &index) const;
    QSize sizeHint(const QStyleOptionViewItem &option,
                   const QModelIndex &index) const;

private:
};

class KNMusicResultView : public QTreeView
{
    Q_OBJECT
public:
    explicit KNMusicResultView(QWidget *parent = 0);

signals:

public slots:

private:

};

#endif // KNMUSICRESULTVIEW_H
