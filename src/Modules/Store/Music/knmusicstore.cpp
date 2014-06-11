#include <QBoxLayout>
#include <QLineEdit>

#include "knmusiconlinenetease.h"
#include "knmusicsearchresult.h"

#include "knmusicstore.h"

KNMusicStore::KNMusicStore(QWidget *parent) :
    QWidget(parent)
{
    m_layout=new QBoxLayout(QBoxLayout::TopToBottom, this);
    setLayout(m_layout);
    m_keywords=new QLineEdit(this);
    m_layout->addWidget(m_keywords);
    m_result=new KNMusicSearchResult(this);
    m_layout->addWidget(m_result, 1);

    KNMusicOnlineNetease *neteaseBackend=new KNMusicOnlineNetease(this);
    connect(m_keywords, &QLineEdit::editingFinished,
            [=]{neteaseBackend->searchMusic(m_keywords->text());
                m_result->setModel(neteaseBackend->model());
                m_result->setFocus();});
}
