#include <QBoxLayout>
#include <QLabel>
#include <QPainter>
#include <QTextDocument>

#include "knmusicsearchresult.h"

void KNMusicHTMLDelegate::paint(QPainter *painter,
                                const QStyleOptionViewItem &option,
                                const QModelIndex &index) const
{
    QStyleOptionViewItemV4 options = option;
    initStyleOption(&options, index);

    painter->save();

    //Hack, use rich QTextDocument to draw the data.
    QTextDocument richTextDocument;
    richTextDocument.setHtml(options.text);

    options.text = "";
    options.widget->style()->drawControl(QStyle::CE_ItemViewItem, &options, painter);

    painter->translate(options.rect.left(), options.rect.top());
    QRect clip(0, 0, options.rect.width(), options.rect.height());
    richTextDocument.drawContents(painter, clip);

    painter->restore();
}

QSize KNMusicHTMLDelegate::sizeHint ( const QStyleOptionViewItem & option, const QModelIndex & index ) const
{
    QStyleOptionViewItemV4 options = option;
    initStyleOption(&options, index);

    QTextDocument doc;
    doc.setHtml(options.text);
    doc.setTextWidth(options.rect.width());
    return QSize(doc.idealWidth(), doc.size().height());
}

KNMusicSearchResult::KNMusicSearchResult(QWidget *parent) :
    QListView(parent)
{
    KNMusicHTMLDelegate *delegate=new KNMusicHTMLDelegate();
    setItemDelegate(delegate);
}
