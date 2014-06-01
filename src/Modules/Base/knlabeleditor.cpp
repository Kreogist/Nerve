#include <QMouseEvent>
#include <QLineEdit>

#include "knlabeleditor.h"

KNLabelEditor::KNLabelEditor(QWidget *parent) :
    QLabel(parent)
{
    m_editor=new QLineEdit(this);
    m_editor->setFrame(false);
    m_editor->hide();
    connect(m_editor, SIGNAL(editingFinished()),
            this, SLOT(setEdited()));
    connect(m_editor, SIGNAL(returnPressed()),
            this, SLOT(setEdited()));
}

KNLabelEditor::KNLabelEditor(const QString &caption, QWidget *parent) :
    KNLabelEditor(parent)
{
    setText(caption);
}

void KNLabelEditor::mouseReleaseEvent(QMouseEvent *event)
{
    if(rect().contains(event->pos()))
    {
        switch (event->button()) {
        case Qt::LeftButton:
            m_editor->setGeometry(0,0,width(),height());
            m_editor->setText(text());
            m_editor->show();
            break;
        default:
            break;
        }
    }
}

void KNLabelEditor::setEdited()
{
    setText(m_editor->text());
    m_editor->hide();
}
