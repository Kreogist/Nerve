#ifndef KNLIBVIEWERCATEGORYBUTTON_H
#define KNLIBVIEWERCATEGORYBUTTON_H

#include <QWidget>
#include <QPushButton>

class QLabel;
class KNLibViewerCategoryButton : public QWidget
{
    Q_OBJECT
public:
    explicit KNLibViewerCategoryButton(QWidget *parent = 0);
    void setIcon(const QPixmap &value);
    void setText(const QString &value);
    bool isChecked() const;
    void setChecked(bool isChecked);

signals:
    void clicked();
    void toggled(bool checked);

public slots:

protected:
    virtual void onCheckedChanged()=0;
    QLabel *m_icon, *m_caption;
    bool m_checked=false;
};

#endif // KNLIBVIEWERCATEGORYBUTTON_H
