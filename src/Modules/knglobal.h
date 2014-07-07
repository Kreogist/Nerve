#ifndef KNGLOBAL_H
#define KNGLOBAL_H

#include <QObject>
#include <QWidget>
#include <QDateTime>
#include <QTextCodec>

class QClipboard;
class KNConfigure;
class KNGlobal : public QObject
{
    Q_OBJECT
public:
    static KNGlobal *instance();

    QString byteToHigher(const qint64 &fileSize);

    QWidget *mainWindow() const;
    void setMainWindow(QWidget *mainWindow);
    static void showInGraphicalShell(const QString &filePath);
    static QString applicationName();
    static void openLocalUrl(const QString &filePath);
    static void uint32ToChars(const quint32 &value, char *item);
    static QString dateTimeToDisplayString(const QDateTime &dateTime);
    static quint32 charsToUint32(const char *value);
    void copyFileToClipboard(const QStringList &files);
    void copyTextToClipboard(const QString &text);
    QString databaseFolder() const;
    QTextCodec *codecForCurrentLocale() const;
    QStringList arguments() const;

signals:

public slots:

private:
    KNGlobal();
#ifdef Q_OS_LINUX
    static QString substituteFileBrowserParameters(QString &pre, QString &file);
#endif
    static KNGlobal *m_instance;
    static QString m_applicationName;
    KNConfigure *m_configure;
    QString m_storageUnit[5], m_libraryPath;
    QWidget *m_mainWindow;
    QClipboard *m_clipboard;
    QStringList m_arguments;
};

#endif // KNGLOBAL_H
