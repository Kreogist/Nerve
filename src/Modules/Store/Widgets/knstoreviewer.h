#ifndef KNSTOREVIEWER_H
#define KNSTOREVIEWER_H

#include "../../Base/knstdlibviewer.h"

class KNMusicStore;
class KNStoreViewer : public KNStdLibViewer
{
    Q_OBJECT
public:
    explicit KNStoreViewer(QWidget *parent = 0);

signals:

public slots:

private:
    KNMusicStore *m_musicStore;
};

#endif // KNSTOREVIEWER_H
