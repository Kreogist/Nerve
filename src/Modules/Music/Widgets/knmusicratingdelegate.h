#ifndef KNMUSICRATINGDELEGATE_H
#define KNMUSICRATINGDELEGATE_H

#include <QStyledItemDelegate>

class KNMusicRatingDelegate : public QStyledItemDelegate
{
    Q_OBJECT
public:
    explicit KNMusicRatingDelegate(QWidget *parent = 0);

    void paint(QPainter *painter,
               const QStyleOptionViewItem &option,
               const QModelIndex &index) const;
    QSize sizeHint(const QStyleOptionViewItem &option,
                   const QModelIndex &index) const;
    QWidget *createEditor(QWidget *parent,
                          const QStyleOptionViewItem &option,
                          const QModelIndex &index) const;
    void setEditorData(QWidget *editor,
                       const QModelIndex &index) const;
    void setModelData(QWidget *editor,
                      QAbstractItemModel *model,
                      const QModelIndex &index) const;
signals:

public slots:

private slots:
    void commitAndCloseEditor();

private:

};

#endif // KNMUSICRATINGDELEGATE_H
