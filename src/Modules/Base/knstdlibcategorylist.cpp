#include <QBoxLayout>
#include <QTimeLine>
#include <QPropertyAnimation>
#include <QSignalMapper>

#include <QDebug>

#include "knstdlibcategorylistbutton.h"
#include "knstdlibcategorybutton.h"
#include "knsideshadows.h"

#include "knstdlibcategorylist.h"

KNStdLibCategoryList::KNStdLibCategoryList(QWidget *parent) :
    KNLibCategoryList(parent)
{
    //Initial Properties.
    setAutoFillBackground(true);
    setContentsMargins(0,0,0,0);
    setFixedHeight(0);
    QPalette pal=palette();
    pal.setColor(QPalette::Window, QColor(0x29, 0x2a, 0x2e));
    setPalette(pal);

    //Button for showing category list.
    m_button=new KNStdLibCategoryButton;
    connect(m_button, &KNStdLibCategoryButton::requireShowCategorySelect,
            this, &KNStdLibCategoryList::showCategorySwitcher);
    connect(m_button, &KNStdLibCategoryButton::requireHideCategorySelect,
            this, &KNStdLibCategoryList::hideCategorySwitcher);

    m_layout=new QBoxLayout(QBoxLayout::TopToBottom, this);
    m_layout->setContentsMargins(0,0,0,0);
    m_layout->setSpacing(0);
    setLayout(m_layout);

    //Initial select signal mapper.
    m_selectMapper=new QSignalMapper(this);
    connect(m_selectMapper, SIGNAL(mapped(int)),
            this, SLOT(onActionSwitch(int)));

    KNTopSideShadow *topShadow=new KNTopSideShadow(this);
    topShadow->setMaximumHeight(15);
    m_layout->addWidget(topShadow, 1);

    m_buttonLayout=new QBoxLayout(QBoxLayout::LeftToRight,
                                  m_layout->widget());
    m_buttonLayout->setContentsMargins(10,0,10,0);
    m_buttonLayout->setSpacing(10);
    m_buttonLayout->addStretch();
    m_layout->addStretch();
    m_layout->addLayout(m_buttonLayout, 1);
    m_layout->addStretch();

    KNBottomSideShadow *bottomShadow=new KNBottomSideShadow(this);
    bottomShadow->setMaximumHeight(15);
    m_layout->addWidget(bottomShadow, 1);

    m_expandCategory=new QTimeLine(200, this);
    m_expandCategory->setUpdateInterval(10);
    m_expandCategory->setEndFrame(170);
    connect(m_expandCategory, &QTimeLine::frameChanged,
            this, &KNStdLibCategoryList::setFixedHeight);
    connect(m_expandCategory, &QTimeLine::valueChanged,
            this, &KNStdLibCategoryList::requireChangeOpacity);

    m_foldCategory=new QTimeLine(200, this);
    m_foldCategory->setUpdateInterval(10);
    m_foldCategory->setEndFrame(0);
    connect(m_foldCategory, &QTimeLine::frameChanged,
            this, &KNStdLibCategoryList::setFixedHeight);
    connect(m_foldCategory, &QTimeLine::finished,
            [=]
            {
                emit requireEnableContent();
                if(m_switchToIndex!=-1)
                {
                    emit requireSwitchTo(m_switchToIndex);
                    m_switchToIndex=-1;
                }
            });
    connect(m_foldCategory, &QTimeLine::valueChanged,
            [=](const qreal &value)
            {
                emit requireChangeOpacity(1.0-value);
            });
}

void KNStdLibCategoryList::addCategory(const QString &title,
                                       const QPixmap &icon)
{
    CategoryItem item;
    item.title=title;
    item.icon=icon;
    if(m_categories.isEmpty())
    {
        //This is the first item.
        m_currentIndex=0;
        m_button->setCategoryIcon(icon);
        m_button->setCategoryText(title);
    }
    //Create category button.
    KNStdLibCategoryListButton *currentButton
            =new KNStdLibCategoryListButton(this);
    currentButton->setPixmap(icon);
    //Set signal map to category select.
    connect(currentButton, SIGNAL(clicked()),
            m_selectMapper, SLOT(map()));
    m_selectMapper->setMapping(currentButton, m_categories.size());
    //Insert widget to the layout.
    m_buttonLayout->insertWidget(m_buttonLayout->count()-1,
                                 currentButton);
    //Add to list.
    m_categories.append(item);
}

void KNStdLibCategoryList::showCategorySwitcher()
{
    emit requireDisableContent();
    m_foldCategory->stop();
    m_expandCategory->setStartFrame(height());
    m_expandCategory->start();
}

void KNStdLibCategoryList::hideCategorySwitcher()
{
    m_expandCategory->stop();
    m_foldCategory->setStartFrame(height());
    m_foldCategory->start();
}

void KNStdLibCategoryList::onActionSwitch(const int &index)
{
    //If the current index is just the user want to switch to,
    //Just hide the category switcher.
    if(m_currentIndex==index)
    {
        hideCategorySwitcher();
        m_button->toNormalModeAndSelectionFolded();
        return;
    }
    CategoryItem currentCategory=m_categories.at(index);
    m_button->onActionSwitchTo(currentCategory.icon,
                               currentCategory.title);
    m_currentIndex=index;
    m_switchToIndex=index;
    hideCategorySwitcher();
}

KNStdLibCategoryButton *KNStdLibCategoryList::listButton()
{
    return m_button;
}
