#pragma once

#include <gui/mgsmainwindow.h>

class CMODeskProcess : public QObject
{
    Q_OBJECT
private:
    MgsMainWindow *m_mainWindow;

public:
    explicit CMODeskProcess(MgsMainWindow *mainWindow, QObject *parent = nullptr);

signals:

public slots:
    void onVDChanged(unsigned int newVD);
    void onKeyPress(unsigned int keyCode);
};

