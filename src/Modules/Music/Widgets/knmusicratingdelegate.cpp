#include <QWidget>
#include <QPainter>
#include <QVariant>

#include "../Libraries/knmusicstarrating.h"
#include "knmusicratingeditor.h"

#include "knmusicratingdelegate.h"

KNMusicRatingDelegate::KNMusicRatingDelegate(QWidget *parent) :
    QStyledItemDelegate(parent)
{
    ;
}

void KNMusicRatingDelegate::paint(QPainter *painter,
                                  const QStyleOptionViewItem &option,
                                  const QModelIndex &index) const
{
    KNMusicStarRating starRating = qvariant_cast<KNMusicStarRating>(index.data());

    if (option.state & QStyle::State_Selected)
    {
        painter->fillRect(option.rect, option.palette.highlight());
    }

    starRating.paint(painter,
                     option.rect,
                     option.palette,
                     KNMusicStarRating::ReadOnly);
}

QSize KNMusicRatingDelegate::sizeHint(const QStyleOptionViewItem &option,
                                      const QModelIndex &index) const
{
    KNMusicStarRating starRating = qvariant_cast<KNMusicStarRating>(index.data());
    return starRating.sizeHint();
}

QWidget *KNMusicRatingDelegate::createEditor(QWidget *parent,
                                             const QStyleOptionViewItem &option,
                                             const QModelIndex &index) const
{
    Q_UNUSED(index);
    KNMusicRatingEditor *editor=new KNMusicRatingEditor(parent);
    connect(editor, &KNMusicRatingEditor::editingFinished,
            this, &KNMusicRatingDelegate::commitAndCloseEditor);
    return editor;
}

void KNMusicRatingDelegate::setEditorData(QWidget *editor,
                                          const QModelIndex &index) const
{
    KNMusicStarRating starRating = qvariant_cast<KNMusicStarRating>(index.data());
    KNMusicRatingEditor *starEditor = qobject_cast<KNMusicRatingEditor *>(editor);
    starEditor->setStarRating(starRating);
}

void KNMusicRatingDelegate::setModelData(QWidget *editor,
                                         QAbstractItemModel *model,
                                         const QModelIndex &index) const
{
    KNMusicRatingEditor *starEditor = static_cast<KNMusicRatingEditor *>(editor);
    model->setData(index, QVariant::fromValue(starEditor->starRating()));
}

void KNMusicRatingDelegate::commitAndCloseEditor()
{
    KNMusicRatingEditor *editor=qobject_cast<KNMusicRatingEditor *>(sender());
    emit commitData(editor);
    emit closeEditor(editor);
}
