#ifndef KNLABELEDITOR_H
#define KNLABELEDITOR_H

#include <QLabel>

class QLineEdit;
class QResizeEvent;
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
    void resizeEvent(QResizeEvent *event);

private slots:
    void setEdited();
    void cancelEdited();

private:
    QLineEdit *m_editor;
};

#endif // KNLABELEDITOR_H
