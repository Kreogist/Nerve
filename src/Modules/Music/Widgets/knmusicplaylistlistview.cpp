#include <QBoxLayout>
#include <QMouseEvent>
#include <QKeyEvent>
#include <QSignalMapper>
#include <QResizeEvent>
#include <QPropertyAnimation>
#include <QTimeLine>
#include <QEvent>

#include <QDebug>

#include "knmusicplaylistlistview.h"

KNMusicPlaylistItemEditor::KNMusicPlaylistItemEditor(QWidget *parent) :
    QLineEdit(parent)
{
    //Set properties.
    setAutoFillBackground(true);
    setClearButtonEnabled(true);
    setFrame(QFrame::NoFrame);

    //Set palette.
    QPalette editorPalette=palette();
    editorPalette.setColor(QPalette::Base, QColor(0,0,0,0));
    editorPalette.setColor(QPalette::Window, QColor(0,0,0,0));
    editorPalette.setColor(QPalette::Text, QColor(255,255,255));
    setPalette(editorPalette);
}

void KNMusicPlaylistItemEditor::keyReleaseEvent(QKeyEvent *event)
{
    switch(event->key())
    {
    case Qt::Key_Escape:
        emit cancelEdit();
        break;
    case Qt::Key_Enter:
    case Qt::Key_Return:
        emit ensureEdit();
        break;
    default:
        QLineEdit::keyReleaseEvent(event);
    }
}

KNMusicPlaylistListviewItem::KNMusicPlaylistListviewItem(QWidget *parent) :
    QWidget(parent)
{
    //Set properties.
    setContentsMargins(0,0,0,0);
    setAutoFillBackground(true);
    setFixedHeight(m_itemHeight);
    setSizePolicy(QSizePolicy::MinimumExpanding,
                  QSizePolicy::Fixed);

    //Set palette.
    m_palette=palette();
    m_palette.setColor(QPalette::Window, QColor(0,0,0,0));
    m_palette.setColor(QPalette::WindowText, m_textColor);
    m_textPalette=m_palette;
    setPalette(m_palette);

    //Set layout.
    m_layout=new QBoxLayout(QBoxLayout::LeftToRight, this);
    m_layout->setContentsMargins(0,0,0,0);
    setLayout(m_layout);
    m_icon=new QLabel(this);
    m_layout->addWidget(m_icon);
    m_text=new QLabel(this);
    m_layout->addWidget(m_text);
    m_text->setPalette(m_textPalette);
    m_layout->addStretch();

    //Initial the name editor
    m_editor=new KNMusicPlaylistItemEditor(this);
    connect(m_editor, &KNMusicPlaylistItemEditor::cancelEdit,
            this, &KNMusicPlaylistListviewItem::onActionCancelEdit);
    connect(m_editor, &KNMusicPlaylistItemEditor::ensureEdit,
            this, &KNMusicPlaylistListviewItem::onActionEnsureEdit);
    m_editor->hide();

    //Initial time line.
    m_createAnime=new QTimeLine(100, this);
    m_createAnime->setUpdateInterval(5);
    m_createAnime->setFrameRange(0, m_itemHeight);
    m_createAnime->setEasingCurve(QEasingCurve::OutCubic);
    connect(m_createAnime, &QTimeLine::frameChanged,
            this, &KNMusicPlaylistListviewItem::onActionReheight);

    m_deleteAnime=new QTimeLine(100, this);
    m_deleteAnime->setUpdateInterval(5);
    m_deleteAnime->setFrameRange(m_itemHeight, 0);
    m_deleteAnime->setEasingCurve(QEasingCurve::OutCubic);
    connect(m_deleteAnime, &QTimeLine::frameChanged,
            this, &KNMusicPlaylistListviewItem::onActionReheight);
    connect(m_deleteAnime, &QTimeLine::finished,
            this, &KNMusicPlaylistListviewItem::requireDelete);

    m_editorInAnime=new QPropertyAnimation(m_editor, "pos", this);
    m_editorInAnime->setEasingCurve(QEasingCurve::OutCubic);
    connect(m_createAnime, &QTimeLine::finished,
            this, &KNMusicPlaylistListviewItem::onActionCreateFinished);
}

void KNMusicPlaylistListviewItem::setText(const QString &text)
{
    m_text->setText(text);
}

void KNMusicPlaylistListviewItem::setData(const QVariant &data)
{
    m_data=data;
}

void KNMusicPlaylistListviewItem::setCreateMode()
{
    //prepartion for create item mode.
    m_createMode=true;
    setFixedHeight(0);
}

void KNMusicPlaylistListviewItem::startCreateMode()
{
    m_createAnime->start();
}

void KNMusicPlaylistListviewItem::select()
{
    m_unselected=false;
    m_palette.setColor(QPalette::Window, QColor(0x60, 0x60, 0x60));
    setPalette(m_palette);
    m_textPalette.setColor(QPalette::WindowText, QColor(0xf7, 0xcf, 0x3d));
    m_text->setPalette(m_textPalette);
}

void KNMusicPlaylistListviewItem::unselect()
{
    m_unselected=true;
    m_palette.setColor(QPalette::Window, QColor(0,0,0,0));
    setPalette(m_palette);
    m_textPalette.setColor(QPalette::WindowText, m_textColor);
    m_text->setPalette(m_textPalette);
}

QVariant KNMusicPlaylistListviewItem::data() const
{
    return m_data;
}

void KNMusicPlaylistListviewItem::setTextColorParam(const int &frame)
{
    m_textColor=QColor(frame, frame, frame);
    if(m_unselected)
    {
        m_textPalette.setColor(QPalette::WindowText, m_textColor);
        m_text->setPalette(m_textPalette);
    }
}

void KNMusicPlaylistListviewItem::mousePressEvent(QMouseEvent *event)
{
    QWidget::mousePressEvent(event);
    m_isPressed=true;
}

void KNMusicPlaylistListviewItem::mouseReleaseEvent(QMouseEvent *event)
{
    QWidget::mouseReleaseEvent(event);
    if(m_isPressed)
    {
        m_isPressed=false;
        emit itemClicked();
    }
}

void KNMusicPlaylistListviewItem::onActionReheight(const int &itemHeight)
{
    setFixedHeight(itemHeight);
}

void KNMusicPlaylistListviewItem::onActionCancelEdit()
{
    if(m_createMode)
    {
        onActionAnimeDelete();
        return;
    }
}

void KNMusicPlaylistListviewItem::onActionEnsureEdit()
{
    m_text->setText(m_editor->text());
    m_editor->hide();
    if(m_createMode)
    {
        emit requireAdd(m_text->text());
    }
}

void KNMusicPlaylistListviewItem::onActionCreateFinished()
{
    m_editor->resize(width()-m_text->x(),
                     height());
    m_editor->setPlaceholderText(tr("New Playlist"));
    m_editorInAnime->setStartValue(QPoint(-m_editor->width(),
                                          0));
    m_editorInAnime->setEndValue(QPoint(m_text->x(),
                                        0));
    m_editor->show();
    m_editor->setFocus();
    m_editorInAnime->start();
}

void KNMusicPlaylistListviewItem::onActionAnimeDelete()
{
    m_editor->hide();
    m_deleteAnime->start();
}

KNMusicPlaylistListviewHeader::KNMusicPlaylistListviewHeader(QWidget *parent) :
    QLabel(parent)
{
    //Set properties.
    setAutoFillBackground(true);
    setContentsMargins(10, 0, 0, 0);
    setFixedHeight(30);

    //Set palette.
    m_palette=palette();
    m_background.setStart(0, 0);
    m_background.setColorAt(0, QColor(0x2d, 0x2d, 0x2d));
    m_background.setColorAt(1, QColor(0x2d, 0x2d, 0x2d));
    m_palette.setColor(QPalette::WindowText, QColor(255,255,255));
    m_palette.setBrush(QPalette::Window, m_background);
    setPalette(m_palette);
}

KNMusicPlaylistListviewHeader::~KNMusicPlaylistListviewHeader()
{
    clear();
}

int KNMusicPlaylistListviewHeader::addItem(KNMusicPlaylistListviewItem *item)
{
    int itemIndex=m_items.size();
    connect(item, &KNMusicPlaylistListviewItem::itemClicked,
            this, &KNMusicPlaylistListviewHeader::onActionItemClicked);
    m_items.append(item);
    m_itemLayout->addWidget(item);
    return itemIndex;
}

void KNMusicPlaylistListviewHeader::removeItem(const int &index)
{
    KNMusicPlaylistListviewItem *item=m_items.takeAt(index);
    item->deleteLater();
}

void KNMusicPlaylistListviewHeader::animateRemoveItem(const int &index)
{
    m_animeRemovedIndex=index;
    m_items.at(m_animeRemovedIndex)->onActionAnimeDelete();
}

void KNMusicPlaylistListviewHeader::clear()
{
    int itemSize=m_items.size();
    while(itemSize--)
    {
        m_items.takeLast()->deleteLater();
    }
}

int KNMusicPlaylistListviewHeader::itemCount()
{
    return m_items.size();
}

void KNMusicPlaylistListviewHeader::selectItem(const int &index)
{
    m_items.at(index)->select();
}

void KNMusicPlaylistListviewHeader::unselectItem(const int &index)
{
    m_items.at(index)->unselect();
}

void KNMusicPlaylistListviewHeader::setStartParam(const int &param)
{
    int colorParam=0x2d+param;
    m_background.setColorAt(0, QColor(colorParam,
                                      colorParam,
                                      colorParam));
    m_palette.setBrush(QPalette::Window, m_background);
    setPalette(m_palette);
}

void KNMusicPlaylistListviewHeader::setItemLayout(QBoxLayout *itemLayout)
{
    m_itemLayout=itemLayout;
}

void KNMusicPlaylistListviewHeader::resizeEvent(QResizeEvent *event)
{
    QLabel::resizeEvent(event);
    m_background.setFinalStop(0, event->size().height());
    m_palette.setBrush(QPalette::Window, m_background);
    setPalette(m_palette);
}

void KNMusicPlaylistListviewHeader::onActionItemClicked()
{
    KNMusicPlaylistListviewItem *clickedItem=
            static_cast<KNMusicPlaylistListviewItem *>(sender());
    emit itemClicked(m_headerIndex,
                     m_items.indexOf(clickedItem));
}
int KNMusicPlaylistListviewHeader::headerIndex() const
{
    return m_headerIndex;
}

void KNMusicPlaylistListviewHeader::setHeaderIndex(int headerIndex)
{
    m_headerIndex=headerIndex;
}

void KNMusicPlaylistListviewHeader::onActionDeleteAnimeFinished()
{
    removeItem(m_animeRemovedIndex);
}

KNMusicPlaylistListviewContent::KNMusicPlaylistListviewContent(QWidget *parent) :
    QWidget(parent)
{
    //Set properties.
    setContentsMargins(0,0,0,0);
    setSizePolicy(QSizePolicy::MinimumExpanding,
                  QSizePolicy::MinimumExpanding);

    QBoxLayout *stretchLayout=new QBoxLayout(QBoxLayout::TopToBottom,
                                             this);
    stretchLayout->setContentsMargins(0,0,0,0);
    stretchLayout->setSpacing(0);
    setLayout(stretchLayout);

    m_mainLayout=new QBoxLayout(QBoxLayout::TopToBottom,
                                stretchLayout->widget());
    m_mainLayout->setContentsMargins(0,0,0,0);
    m_mainLayout->setSpacing(0);
    stretchLayout->addLayout(m_mainLayout);
    stretchLayout->addStretch();
}

KNMusicPlaylistListviewContent::~KNMusicPlaylistListviewContent()
{
    clear();
}

int KNMusicPlaylistListviewContent::addHeader(const QString &text)
{
    KNMusicPlaylistListviewHeader *currentHeader=new KNMusicPlaylistListviewHeader();
    int headerIndex=m_headers.size();
    currentHeader->setHeaderIndex(headerIndex);
    currentHeader->setText(text);
    connect(this, &KNMusicPlaylistListviewContent::requireSetHeaderColorParam,
            currentHeader, &KNMusicPlaylistListviewHeader::setStartParam);
    connect(currentHeader, &KNMusicPlaylistListviewHeader::itemClicked,
            this, &KNMusicPlaylistListviewContent::onActionItemClicked);
    m_mainLayout->addWidget(currentHeader);
    QBoxLayout *headerItemLayout=new QBoxLayout(QBoxLayout::TopToBottom,
                                                m_mainLayout->widget());
    m_mainLayout->addLayout(headerItemLayout);
    currentHeader->setItemLayout(headerItemLayout);
    m_headers.append(currentHeader);
    m_opertateHeader=currentHeader;
    return headerIndex;
}

int KNMusicPlaylistListviewContent::addItem(const QString &text)
{
    KNMusicPlaylistListviewItem *currentItem=new KNMusicPlaylistListviewItem();
    currentItem->setText(text);
    connect(currentItem, &KNMusicPlaylistListviewItem::requireDelete,
            m_opertateHeader, &KNMusicPlaylistListviewHeader::onActionDeleteAnimeFinished);
    connect(this, &KNMusicPlaylistListviewContent::requireSetTextColorParam,
            currentItem, &KNMusicPlaylistListviewItem::setTextColorParam);
    return m_opertateHeader->addItem(currentItem);
}

int KNMusicPlaylistListviewContent::currentHeader() const
{
    return m_currentHeader;
}

int KNMusicPlaylistListviewContent::currentItem() const
{
    return m_currentItem;
}

void KNMusicPlaylistListviewContent::createItem()
{
    if(m_createdIndex==-1)
    {
        m_createdItem=new KNMusicPlaylistListviewItem();
        m_createdItem->setCreateMode();
        connect(this, &KNMusicPlaylistListviewContent::requireSetTextColorParam,
                m_createdItem, &KNMusicPlaylistListviewItem::setTextColorParam);
        connect(m_createdItem, &KNMusicPlaylistListviewItem::requireDelete,
                this, &KNMusicPlaylistListviewContent::createCancel);
        connect(m_createdItem, &KNMusicPlaylistListviewItem::requireAdd,
                this, &KNMusicPlaylistListviewContent::createApply);
        m_createdIndex=m_opertateHeader->addItem(m_createdItem);
        m_createdItem->startCreateMode();
    }
}

void KNMusicPlaylistListviewContent::removeCurrentItem()
{
    if(m_currentHeader!=-1 && m_currentItem!=-1)
    {
        KNMusicPlaylistListviewHeader *header=m_headers.at(m_currentHeader);
        int nextSelectedItem=m_currentItem-1,
            currentBackup=m_currentItem;
        if(nextSelectedItem>-1)
        {
            setCurrentItem(m_currentHeader, nextSelectedItem);
        }
        else
        {
            m_currentItem=-1;
        }
        header->animateRemoveItem(currentBackup);
    }
}

void KNMusicPlaylistListviewContent::clear()
{
    //Delete all the header widget.
    int itemSize=m_headers.size();
    while(itemSize--)
    {
        m_headers.takeLast()->deleteLater();
    }
}

void KNMusicPlaylistListviewContent::clearHeader()
{
    m_opertateHeader->clear();
}

bool KNMusicPlaylistListviewContent::setCurrentHeader(const int &index)
{
    if(index>-1 && index<m_headers.size())
    {
        m_currentHeader=index;
        m_opertateHeader=m_headers.at(index);
        return true;
    }
    return false;
}

void KNMusicPlaylistListviewContent::setHeaderColorParam(const int &param)
{
    emit requireSetHeaderColorParam(param);
}

void KNMusicPlaylistListviewContent::setTextColorParam(const int &param)
{
    emit requireSetTextColorParam(param);
}

void KNMusicPlaylistListviewContent::setCurrentItem(const int &header,
                                                    const int &item)
{
    if(m_currentItem!=-1)
    {
        KNMusicPlaylistListviewHeader *lastHeader=m_headers.at(m_currentHeader);
        //An item has been selected, unselected it.
        if(m_currentItem<lastHeader->itemCount())
        {
            lastHeader->unselectItem(m_currentItem);
        }
    }
    m_currentHeader=header;
    m_currentItem=item;
    m_headers.at(m_currentHeader)->selectItem(m_currentItem);
}

void KNMusicPlaylistListviewContent::createCancel()
{
    disconnectCreatedItem();
    m_opertateHeader->removeItem(m_createdIndex);
    m_createdItem=nullptr;
    m_createdIndex=-1;
}

void KNMusicPlaylistListviewContent::createApply(const QString &caption)
{
    disconnectCreatedItem();
    //UI text color change signal.
    connect(this, &KNMusicPlaylistListviewContent::requireSetTextColorParam,
            m_createdItem, &KNMusicPlaylistListviewItem::setTextColorParam);
    //Redirect the delete signal.
    connect(m_createdItem, &KNMusicPlaylistListviewItem::requireDelete,
            m_opertateHeader, &KNMusicPlaylistListviewHeader::onActionDeleteAnimeFinished);
    m_createdItem=nullptr;
    emit requireAddPlaylist(caption);
    setCurrentItem(m_currentHeader, m_createdIndex);
    m_createdIndex=-1;
}

void KNMusicPlaylistListviewContent::onActionItemClicked(const int &header,
                                                         const int &item)
{
    setCurrentItem(header, item);
    emit itemClicked(header, item);
}

void KNMusicPlaylistListviewContent::disconnectCreatedItem()
{
    disconnect(m_createdItem, &KNMusicPlaylistListviewItem::requireDelete,
               this, &KNMusicPlaylistListviewContent::createCancel);
    disconnect(m_createdItem, &KNMusicPlaylistListviewItem::requireAdd,
               this, &KNMusicPlaylistListviewContent::createApply);
    disconnect(this, &KNMusicPlaylistListviewContent::requireSetTextColorParam,
               m_createdItem, &KNMusicPlaylistListviewItem::setTextColorParam);
}

KNMusicPlaylistListview::KNMusicPlaylistListview(QWidget *parent) :
    QScrollArea(parent)
{
    //Set properties.
    setWidgetResizable(true);
    setFrameShape(QFrame::NoFrame);
    viewport()->setContentsMargins(0,0,0,0);

    //Set palette.
    m_palette=palette();
    m_baseColor=QColor(0,0,0);
    m_palette.setColor(QPalette::WindowText, QColor(m_textBaseParam,
                                                    m_textBaseParam,
                                                    m_textBaseParam));
    m_palette.setColor(QPalette::Window, m_baseColor);
    setPalette(m_palette);

    //Set content widget.
    m_content=new KNMusicPlaylistListviewContent(this);
    connect(m_content, &KNMusicPlaylistListviewContent::requireAddPlaylist,
            this, &KNMusicPlaylistListview::requireAddPlaylist);
    connect(m_content, &KNMusicPlaylistListviewContent::itemClicked,
            this, &KNMusicPlaylistListview::itemClicked);
    setWidget(m_content);

    //Mouse enter animation.
    m_mouseIn=new QTimeLine(200, this);
    m_mouseIn->setUpdateInterval(5);
    m_mouseIn->setEndFrame(0x10);
    m_mouseIn->setEasingCurve(QEasingCurve::OutCubic);
    connect(m_mouseIn, &QTimeLine::frameChanged,
            this, &KNMusicPlaylistListview::onActionChangeBackground);

    //Moues leave animation.
    m_mouseOut=new QTimeLine(200, this);
    m_mouseOut->setUpdateInterval(5);
    m_mouseOut->setEndFrame(0x0);
    m_mouseOut->setEasingCurve(QEasingCurve::OutCubic);
    connect(m_mouseOut, &QTimeLine::frameChanged,
            this, &KNMusicPlaylistListview::onActionChangeBackground);
}

int KNMusicPlaylistListview::addHeader(const QString &text)
{
    return m_content->addHeader(text);
}

int KNMusicPlaylistListview::addItem(const QString &text)
{
    return m_content->addItem(text);
}

int KNMusicPlaylistListview::currentHeader() const
{
    return m_content->currentHeader();
}

int KNMusicPlaylistListview::currentItem() const
{
    return m_content->currentItem();
}

void KNMusicPlaylistListview::clearHeader()
{
    m_content->clearHeader();
}

void KNMusicPlaylistListview::removeCurrentItem()
{
    m_content->removeCurrentItem();
}

bool KNMusicPlaylistListview::setCurrentHeader(const int &index)
{
    return m_content->setCurrentHeader(index);
}

void KNMusicPlaylistListview::createItem()
{
    m_content->createItem();
}

void KNMusicPlaylistListview::enterEvent(QEvent *event)
{
    QScrollArea::enterEvent(event);
    m_mouseOut->stop();
    m_mouseIn->setStartFrame(m_baseColor.red());
    m_mouseIn->start();
}

void KNMusicPlaylistListview::leaveEvent(QEvent *event)
{
    QScrollArea::leaveEvent(event);
    m_mouseIn->stop();
    m_mouseOut->setStartFrame(m_baseColor.red());
    m_mouseOut->start();
}

void KNMusicPlaylistListview::onActionChangeBackground(const int &frame)
{
    m_baseColor=QColor(frame, frame, frame);
    m_palette.setColor(QPalette::Window, m_baseColor);
    setPalette(m_palette);
    m_content->setHeaderColorParam(frame<<1);
    m_content->setTextColorParam((frame<<3)+m_textBaseParam);
}
