#ifndef KNSTDLIBFILEEXPLORERPATH_H
#define KNSTDLIBFILEEXPLORERPATH_H

#include "knlibfileexplorerpath.h"

class QBoxLayout;
class KNStdLibFileExplorerPath : public KNLibFileExplorerPath
{
    Q_OBJECT
public:
    explicit KNStdLibFileExplorerPath(QWidget *parent = 0);

signals:

public slots:

private:
    QBoxLayout *m_mainLayout;
};

#endif // KNSTDLIBFILEEXPLORERPATH_H
