#ifndef KNLIBVIEWERCATEGORY_H
#define KNLIBVIEWERCATEGORY_H

#include <QStringList>
#include <QWidget>

class KNLibViewerCategory : public QWidget
{
    Q_OBJECT
public:
    explicit KNLibViewerCategory(QWidget *parent = 0);
    virtual void addCategory(const QPixmap &icon, const QString &category)=0;

signals:
    void categoryChanged(int index);

public slots:

private:
};

#endif // KNLIBVIEWERCATEGORY_H
