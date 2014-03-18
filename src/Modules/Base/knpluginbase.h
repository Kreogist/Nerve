#ifndef KNPLUGINBASE_H
#define KNPLUGINBASE_H

#include <QObject>

class QWidget;
class KNPluginBase : public QObject
{
    Q_OBJECT
public:
    explicit KNPluginBase(QObject *parent = 0);
    virtual void applyPlugin();

signals:
    void requireAddCategory(const QString &title,
                            const QPixmap &icon,
                            QWidget *widget);

public slots:

};

#endif // KNPLUGINBASE_H
