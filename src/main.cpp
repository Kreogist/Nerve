#include <QApplication>

#include <QString>
#include <QDebug>
#include <iostream>

#include <QStyleFactory>

#include "mainwindow.h"

/*!
 * \brief This is a custom message handler to deal with Qt's message(for example, qDebug).
 *
 *  KCMessageHandler outputs message which was sended by calling qDebug()(and so on) to both file
 *  (named log.txt) and stderr. This function will be called by Qt.
 * \param type describes the messages that be sent to a message handler
 * \param context This provides additional information about a log message.
 * \param msg message
 */
void KCMessageHandler(QtMsgType type, const QMessageLogContext &context, const QString &msg)
{
    QString message;

    switch(type)
    {
    case QtDebugMsg:
        message.sprintf("Debug: (%s:%u\n%s)\n",
                        context.file,
                        context.line,
                        context.function);
        break;
    case QtWarningMsg:
        message.sprintf("Warning: (%s:%u\n%s)\n",
                        context.file,
                        context.line,
                        context.function);
        break;
    case QtCriticalMsg:
        message.sprintf("Critical: (%s:%u\n%s)\n",
                        context.file,
                        context.line,
                        context.function);
        break;
    case QtFatalMsg:
        message.sprintf("Fatal: (%s:%u\n%s)\n",
                        context.file,
                        context.line,
                        context.function);
    default:
        message.sprintf("Unknow Message Type: (%s:%u\n%s)\n",
                        context.file,
                        context.line,
                        context.function);
    }

    std::cerr<<message.toStdString()<<msg.toStdString()<<std::endl<<std::endl;
}

int main(int argc, char *argv[])
{
//    qInstallMessageHandler(KCMessageHandler);
    QApplication app(argc, argv);
    app.setStyle(QStyleFactory::create("fusion"));
    MainWindow mainWindow;
    mainWindow.show();
    return app.exec();
}
