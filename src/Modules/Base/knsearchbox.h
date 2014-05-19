#ifndef KNSEARCHBOX_H
#define KNSEARCHBOX_H

#include <QWidget>

class QBoxLayout;
class QLineEdit;
class KNSearchBox : public QWidget
{
    Q_OBJECT
public:
    explicit KNSearchBox(QWidget *parent = 0);
    void setPlaceHolderText(const QString &text);

signals:

public slots:

private:
    QPalette m_palette;
    QLineEdit *m_keyText;
    QBoxLayout *m_layout;
};

#endif // KNSEARCHBOX_H
