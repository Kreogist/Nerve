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
    if (index.data().canConvert<KNMusicStarRating>())
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
    else
    {
        QStyledItemDelegate::paint(painter, option, index);
    }
}

QSize KNMusicRatingDelegate::sizeHint(const QStyleOptionViewItem &option,
                                      const QModelIndex &index) const
{
    if (index.data().canConvert<KNMusicStarRating>())
    {
        KNMusicStarRating starRating = qvariant_cast<KNMusicStarRating>(index.data());
        return starRating.sizeHint();
    }
    else
    {
        return QStyledItemDelegate::sizeHint(option, index);
    }
}

QWidget *KNMusicRatingDelegate::createEditor(QWidget *parent,
                                             const QStyleOptionViewItem &option,
                                             const QModelIndex &index) const
{
    if (index.data().canConvert<KNMusicStarRating>())
    {
        KNMusicRatingEditor *editor = new KNMusicRatingEditor(parent);
        connect(editor, SIGNAL(editingFinished()),
                this, SLOT(commitAndCloseEditor()));
        return editor;
    }
    else
    {
        return QStyledItemDelegate::createEditor(parent, option, index);
    }
}

void KNMusicRatingDelegate::setEditorData(QWidget *editor,
                                          const QModelIndex &index) const
{
    if (index.data().canConvert<KNMusicStarRating>())
    {
        KNMusicStarRating starRating = qvariant_cast<KNMusicStarRating>(index.data());
        KNMusicRatingEditor *starEditor = qobject_cast<KNMusicRatingEditor *>(editor);
        starEditor->setStarRating(starRating);
    }
    else
    {
        QStyledItemDelegate::setEditorData(editor, index);
    }
}

void KNMusicRatingDelegate::setModelData(QWidget *editor,
                                         QAbstractItemModel *model,
                                         const QModelIndex &index) const
{
    if (index.data().canConvert<KNMusicStarRating>()) {
        KNMusicRatingEditor *starEditor = static_cast<KNMusicRatingEditor *>(editor);
        model->setData(index, QVariant::fromValue(starEditor->starRating()));
    } else {
        QStyledItemDelegate::setModelData(editor, model, index);
    }
}

void KNMusicRatingDelegate::commitAndCloseEditor()
{
    KNMusicRatingEditor *editor = qobject_cast<KNMusicRatingEditor *>(sender());
    emit commitData(editor);
    emit closeEditor(editor);
}
