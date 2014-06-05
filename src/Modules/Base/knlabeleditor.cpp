#include <QMouseEvent>
#include <QLineEdit>
#include <QAction>
#include <QResizeEvent>
#include <QKeyEvent>

#include <QDebug>

#include "knlabeleditor.h"

KNLabelEditor::KNLabelEditor(QWidget *parent) :
    QLabel(parent)
{
    m_editor=new QLineEdit(this);
    m_editor->setFrame(false);
    m_editor->setAlignment(Qt::AlignCenter);
    m_editor->installEventFilter(this);
    m_editor->hide();

    QAction *enterPressed=new QAction(m_editor);
    enterPressed->setShortcut(QKeySequence(Qt::Key_Enter));
    connect(enterPressed, SIGNAL(triggered()),
            this, SLOT(setEdited()));
    m_editor->addAction(enterPressed);
    QAction *returnPressed=new QAction(m_editor);
    returnPressed->setShortcut(QKeySequence(Qt::Key_Return));
    connect(returnPressed, SIGNAL(triggered()),
            this, SLOT(setEdited()));
    m_editor->addAction(returnPressed);
}

KNLabelEditor::KNLabelEditor(const QString &caption, QWidget *parent) :
    KNLabelEditor(parent)
{
    setText(caption);
}

bool KNLabelEditor::eventFilter(QObject *target, QEvent *event)
{
    if(target==m_editor)
    {
        switch(event->type())
        {
        case QEvent::FocusOut:
            cancelEdited();
            return true;
        case QEvent::KeyRelease:
            QKeyEvent *keyEvent=static_cast<QKeyEvent *>(event);
            if(keyEvent->key()==Qt::Key_Escape)
            {
                cancelEdited();
                return true;
            }
            return QLabel::eventFilter(target, event);
        }
        return QLabel::eventFilter(target, event);
    }
    return QLabel::eventFilter(target, event);
}

void KNLabelEditor::mouseReleaseEvent(QMouseEvent *event)
{
    if(rect().contains(event->pos()))
    {
        switch (event->button()) {
        case Qt::LeftButton:
            m_editor->setGeometry(0,0,width(),height());
            m_editor->setText(text());
            m_editor->selectAll();
            m_editor->show();
            m_editor->setFocus();
            break;
        default:
            break;
        }
    }
}

void KNLabelEditor::resizeEvent(QResizeEvent *event)
{
    QLabel::resizeEvent(event);
    m_editor->resize(event->size());
}

void KNLabelEditor::setEdited()
{
    setText(m_editor->text());
    emit textEdited(text());
    m_editor->hide();
}

void KNLabelEditor::cancelEdited()
{
    m_editor->hide();
    setFocus();
}
