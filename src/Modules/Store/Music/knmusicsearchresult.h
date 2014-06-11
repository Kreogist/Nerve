#ifndef KNMUSICSEARCHRESULT_H
#define KNMUSICSEARCHRESULT_H

#include <QWidget>
#include <QListView>
#include <QStyledItemDelegate>

class QBoxLayout;
class QLabel;

class KNMusicHTMLDelegate : public QStyledItemDelegate
{
protected:
    void paint(QPainter *painter,
               const QStyleOptionViewItem &option,
               const QModelIndex &index ) const;
    QSize sizeHint(const QStyleOptionViewItem &option,
                   const QModelIndex &index ) const;
};

class KNMusicSearchResult : public QListView
{
    Q_OBJECT
public:
    explicit KNMusicSearchResult(QWidget *parent = 0);

signals:

public slots:

private:

};

#endif // KNMUSICSEARCHRESULT_H
