#ifndef KNSTDLIBDIRMODEL_H
#define KNSTDLIBDIRMODEL_H

#include "knlibdirmodel.h"

class KNStdLibDirModel : public KNLibDirModel
{
    Q_OBJECT
public:
    explicit KNStdLibDirModel(QObject *parent = 0);
    void refresh();

signals:

public slots:

protected:

private:
    QDir *m_currentDir;
};

#endif // KNSTDLIBDIRMODEL_H
