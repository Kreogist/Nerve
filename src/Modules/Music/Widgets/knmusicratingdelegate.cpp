#include <QWidget>
#include <QPainter>
#include <QVariant>

#include <QDebug>

#include "knmusicratingeditor.h"

#include "knmusicratingdelegate.h"

KNMusicRatingDelegate::KNMusicRatingDelegate(QWidget *parent) :
    QStyledItemDelegate(parent)
{
    m_star=QPixmap(":/Music/Resources/Music/stars/star.png");
    m_star=m_star.scaled(m_starSizeHint, m_starSizeHint, Qt::KeepAspectRatio);
}

void KNMusicRatingDelegate::paint(QPainter *painter,
                                  const QStyleOptionViewItem &option,
                                  const QModelIndex &index) const
{
    if (option.state & QStyle::State_Selected)
    {
        painter->fillRect(option.rect, option.palette.highlight());
    }

    int starNum=index.data(Qt::DisplayRole).toInt();
    painter->save();
    painter->translate(option.rect.topLeft());
    for(int i=0; i<starNum; i++)
    {
        painter->drawPixmap(QPoint(m_starSizeHint*i, 0),
                            m_star);
    }
    painter->restore();
}

QSize KNMusicRatingDelegate::sizeHint(const QStyleOptionViewItem &option,
                                      const QModelIndex &index) const
{
    Q_UNUSED(option);
    Q_UNUSED(index);
    return QSize(m_starSizeHint*5, m_starSizeHint);
}

QWidget *KNMusicRatingDelegate::createEditor(QWidget *parent,
                                             const QStyleOptionViewItem &option,
                                             const QModelIndex &index) const
{
    Q_UNUSED(option);
    Q_UNUSED(index);
    KNMusicRatingEditor *editor=new KNMusicRatingEditor(parent);
    editor->setStarSizeHint(m_starSizeHint);
    connect(editor, &KNMusicRatingEditor::editingFinished,
            this, &KNMusicRatingDelegate::commitAndCloseEditor);
    return editor;
}

void KNMusicRatingDelegate::setEditorData(QWidget *editor,
                                          const QModelIndex &index) const
{
    KNMusicRatingEditor *starEditor=static_cast<KNMusicRatingEditor *>(editor);
    starEditor->setStarNum(index.data(Qt::DisplayRole).toInt());
}

void KNMusicRatingDelegate::setModelData(QWidget *editor,
                                         QAbstractItemModel *model,
                                         const QModelIndex &index) const
{
    KNMusicRatingEditor *starEditor=static_cast<KNMusicRatingEditor *>(editor);
    model->setData(index, starEditor->starNum(), Qt::DisplayRole);
}

void KNMusicRatingDelegate::commitAndCloseEditor()
{
    QWidget *editor=static_cast<QWidget *>(sender());
    emit commitData(editor);
    emit closeEditor(editor);
}
