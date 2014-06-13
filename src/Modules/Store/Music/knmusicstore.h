#ifndef KNMUSICSTORE_H
#define KNMUSICSTORE_H

#include <QWidget>

class QBoxLayout;
class QLineEdit;
class KNWidgetSwitcher;
class KNMusicResultView;
class KNMusicOnlineNetease;
class KNMusicStore : public QWidget
{
    Q_OBJECT
public:
    explicit KNMusicStore(QWidget *parent = 0);

signals:

public slots:

private slots:
    void onActionModelUpdate();

private:
    KNMusicOnlineNetease *neteaseBackend;
    QLineEdit *m_keywords;
    KNMusicResultView *m_result;
    KNWidgetSwitcher *m_resultSwitcher;
    QBoxLayout *m_layout;
};

#endif // KNMUSICSTORE_H
