#include <QBoxLayout>

#include "../../Base/knwidgetswitcher.h"
#include "../../Base/knsearchbox.h"
#include "knmusiconlinenetease.h"
#include "knmusicresultview.h"

#include "knmusicstore.h"

KNMusicStore::KNMusicStore(QWidget *parent) :
    QWidget(parent)
{
    m_layout=new QBoxLayout(QBoxLayout::TopToBottom, this);
    setLayout(m_layout);
    m_keywords=new KNSearchBox(this);
    m_layout->addWidget(m_keywords);
    m_resultSwitcher=new KNWidgetSwitcher(this);
    m_layout->addWidget(m_resultSwitcher, 1);
    m_result=new KNMusicResultView(this);
    m_resultSwitcher->addWidget(m_result);

    neteaseBackend=new KNMusicOnlineNetease(this);
    connect(m_keywords, &KNSearchBox::returnPressed,
            [=]{neteaseBackend->searchMusic(m_keywords->text());});
    connect(neteaseBackend, &KNMusicOnlineNetease::modelUpdate,
            this, &KNMusicStore::onActionModelUpdate);
}

void KNMusicStore::onActionModelUpdate()
{
    m_result->setModel(neteaseBackend->model());
    m_result->setFocus();
}
