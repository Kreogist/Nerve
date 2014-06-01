#ifndef KNLABELEDITOR_H
#define KNLABELEDITOR_H

#include <QLabel>

class QLineEdit;
class KNLabelEditor : public QLabel
{
    Q_OBJECT
public:
    explicit KNLabelEditor(QWidget *parent = 0);
    KNLabelEditor(const QString &caption, QWidget *parent = 0);

signals:
    void textEdited(const QString &text);

public slots:

protected:
    void mouseReleaseEvent(QMouseEvent *event);

private slots:
    void setEdited();

private:
    QLineEdit *m_editor;
};

#endif // KNLABELEDITOR_H
