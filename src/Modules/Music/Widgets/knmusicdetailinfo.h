#ifndef KNMUSICDETAILINFO_H
#define KNMUSICDETAILINFO_H

#include <QScopedPointer>
#include <QStandardItemModel>
#include <QTreeView>

#include <QDialog>

class QPlainTextEdit;
class QTreeView;
class KNMusicDetailInfo : public QDialog
{
    Q_OBJECT
public:
    explicit KNMusicDetailInfo(QWidget *parent = 0);
    void setFilePath(const QString &filePath);

signals:

public slots:

private:
    QScopedPointer<QStandardItemModel> m_detailModel;
    QPlainTextEdit *m_detailText;
    QTreeView *m_detailViewer;
};

#endif // KNMUSICDETAILINFO_H
