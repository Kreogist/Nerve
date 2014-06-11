#ifndef KNMUSICSTORE_H
#define KNMUSICSTORE_H

#include <QWidget>

class QBoxLayout;
class QLineEdit;
class KNMusicSearchResult;
class KNMusicStore : public QWidget
{
    Q_OBJECT
public:
    explicit KNMusicStore(QWidget *parent = 0);

signals:

public slots:

private:
    QLineEdit *m_keywords;
    KNMusicSearchResult *m_result;
    QBoxLayout *m_layout;
};

#endif // KNMUSICSTORE_H
