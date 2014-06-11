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
    m_palette.setColor(QPalette::WindowText, QColor(0x70, 0x70, 0x70));
    m_textPalette=m_palette;
    setPalette(m_palette);

    //Set layout.
    m_layout=new QBoxLayout(QBoxLayout::LeftToRight, this);
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
    m_originalTextColor=m_textPalette.color(QPalette::WindowText);
    m_textPalette.setColor(QPalette::WindowText, QColor(0xf7, 0xcf, 0x3d));
    m_text->setPalette(m_textPalette);
}

void KNMusicPlaylistListviewItem::unselect()
{
    m_unselected=true;
    m_palette.setColor(QPalette::Window, QColor(0,0,0,0));
    setPalette(m_palette);
    m_textPalette.setColor(QPalette::WindowText, m_originalTextColor);
    m_text->setPalette(m_textPalette);
}

QVariant KNMusicPlaylistListviewItem::data() const
{
    return m_data;
}

void KNMusicPlaylistListviewItem::setTextColorParam(const int &frame)
{
    if(m_unselected)
    {
        m_textPalette.setColor(QPalette::WindowText, QColor(frame, frame, frame));
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
int KNMusicPlaylistListviewItem::headerIndex() const
{
    return m_headerIndex;
}

void KNMusicPlaylistListviewItem::setHeaderIndex(int headerIndex)
{
    m_headerIndex = headerIndex;
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

void KNMusicPlaylistListviewHeader::clear()
{
    int itemSize=m_items.size();
    while(itemSize--)
    {
        m_items.takeLast()->deleteLater();
    }
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
    int clickedIndex=m_items.indexOf(clickedItem);
    if(m_currentIndex!=-1)
    {
        if(m_currentIndex==clickedIndex)
        {
            return;
        }
        KNMusicPlaylistListviewItem *lastItem=m_items.at(m_currentIndex);
        lastItem->unselect();
    }
    m_currentIndex=clickedIndex;
    clickedItem->select();
}
int KNMusicPlaylistListviewHeader::headerIndex() const
{
    return m_headerIndex;
}

void KNMusicPlaylistListviewHeader::setHeaderIndex(int headerIndex)
{
    m_headerIndex = headerIndex;
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
    m_mainLayout->addWidget(currentHeader);
    QBoxLayout *headerItemLayout=new QBoxLayout(QBoxLayout::TopToBottom,
                                                m_mainLayout->widget());
    m_mainLayout->addLayout(headerItemLayout);
    currentHeader->setItemLayout(headerItemLayout);
    m_headers.append(currentHeader);
    m_currentHeader=currentHeader;
    return headerIndex;
}

int KNMusicPlaylistListviewContent::addItem(const QString &text)
{
    KNMusicPlaylistListviewItem *currentItem=new KNMusicPlaylistListviewItem();
    currentItem->setHeaderIndex(m_currentHeader->headerIndex());
    currentItem->setText(text);
    connect(this, &KNMusicPlaylistListviewContent::requireSetTextColorParam,
            currentItem, &KNMusicPlaylistListviewItem::setTextColorParam);
    return m_currentHeader->addItem(currentItem);
}

void KNMusicPlaylistListviewContent::addCreatePlaylist()
{
    setCurrentHeader(0);
    if(m_createdIndex==-1)
    {
        m_createdItem=new KNMusicPlaylistListviewItem();
        m_createdItem->setCreateMode();
        connect(m_createdItem, &KNMusicPlaylistListviewItem::requireDelete,
                this, &KNMusicPlaylistListviewContent::createCancel);
        connect(m_createdItem, &KNMusicPlaylistListviewItem::requireAdd,
                this, &KNMusicPlaylistListviewContent::createApply);
        m_createdIndex=m_currentHeader->addItem(m_createdItem);
        m_createdItem->startCreateMode();
    }
}

void KNMusicPlaylistListviewContent::clear()
{
    int itemSize=m_headers.size();
    while(itemSize--)
    {
        m_headers.takeLast()->deleteLater();
    }
}

void KNMusicPlaylistListviewContent::clearHeader()
{
    m_currentHeader->clear();
}

bool KNMusicPlaylistListviewContent::setCurrentHeader(const int &index)
{
    if(index>-1 && index<m_headers.size())
    {
        m_currentHeader=m_headers.at(index);
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

void KNMusicPlaylistListviewContent::createCancel()
{
    disconnectCreatedItem();
    m_currentHeader->removeItem(m_createdIndex);
    m_createdItem=nullptr;
    m_createdIndex=-1;
}

void KNMusicPlaylistListviewContent::createApply(const QString &caption)
{
    disconnectCreatedItem();
    connect(this, &KNMusicPlaylistListviewContent::requireSetTextColorParam,
            m_createdItem, &KNMusicPlaylistListviewItem::setTextColorParam);
    m_createdItem=nullptr;
    m_createdIndex=-1;
    emit requireAddPlaylist(caption);
}

void KNMusicPlaylistListviewContent::disconnectCreatedItem()
{
    disconnect(m_createdItem, &KNMusicPlaylistListviewItem::requireDelete,
               this, &KNMusicPlaylistListviewContent::createCancel);
    disconnect(m_createdItem, &KNMusicPlaylistListviewItem::requireAdd,
               this, &KNMusicPlaylistListviewContent::createApply);
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
    m_mouseOut->setEasingCurve(QEasingCurve::InCubic);
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

void KNMusicPlaylistListview::clearHeader()
{
    m_content->clearHeader();
}

bool KNMusicPlaylistListview::setCurrentHeader(const int &index)
{
    return m_content->setCurrentHeader(index);
}

void KNMusicPlaylistListview::createPlaylist()
{
    m_content->addCreatePlaylist();
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
