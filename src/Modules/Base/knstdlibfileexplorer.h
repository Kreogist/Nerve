#ifndef KNSTDLIBFILEEXPLORER_H
#define KNSTDLIBFILEEXPLORER_H

#include <QModelIndex>

#include "knlibfileexplorer.h"

class QDirModel;
class QTreeView;
class QBoxLayout;
class KNStdLibFileExplorer : public KNLibFileExplorer
{
    Q_OBJECT
public:
    explicit KNStdLibFileExplorer(QWidget *parent = 0);

signals:

public slots:

private slots:
    void onActionChangePath(const QModelIndex &index);

private:
    QDirModel *m_dirModel;
    QBoxLayout *m_layout;
    QTreeView *m_detailView;
};

#endif // KNSTDLIBFILEEXPLORER_H
