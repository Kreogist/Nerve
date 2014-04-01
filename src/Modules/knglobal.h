#ifndef KNGLOBAL_H
#define KNGLOBAL_H

#include <QObject>
#include <QWidget>

class KNGlobal : public QObject
{
    Q_OBJECT
public:
    static KNGlobal *instance();

    QString byteToHigher(const qint64 &fileSize);

    QWidget *mainWindow() const;
    void setMainWindow(QWidget *mainWindow);
    static void showInGraphicalShell(const QString &filePath);
    static void openLocalUrl(const QString &filePath);
    void copyFileToClipboard(const QStringList &files);
    void copyTextToClipboard(const QString &text);

signals:

public slots:

private:
    KNGlobal();
    static KNGlobal *m_instance;
    QString m_storageUnit[5];
    QWidget *m_mainWindow;
};

#endif // KNGLOBAL_H
