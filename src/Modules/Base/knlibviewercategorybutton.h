#ifndef KNLIBVIEWERCATEGORYBUTTON_H
#define KNLIBVIEWERCATEGORYBUTTON_H

#include <QWidget>

class QLabel;
class KNLibViewerCategoryButton : public QWidget
{
    Q_OBJECT
public:
    explicit KNLibViewerCategoryButton(QWidget *parent = 0);
    virtual QPixmap icon() const=0;
    virtual QString text() const=0;
    virtual void setIcon(const QPixmap &value)=0;
    virtual void setText(const QString &value)=0;
    bool isChecked() const;
    void setChecked(bool isChecked);

signals:
    void clicked();
    void toggled(bool checked);

public slots:

protected:
    virtual void onCheckedChanged()=0;
    bool m_checked=false;
};

#endif // KNLIBVIEWERCATEGORYBUTTON_H
