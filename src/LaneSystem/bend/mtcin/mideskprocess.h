#pragma once

#include <QObject>

class MgsMainWindow;
class MIDeskProcess : public QObject
{
    Q_OBJECT
public:
    explicit MIDeskProcess(MgsMainWindow *mainWindow, QObject *parent = 0);
    ~MIDeskProcess() override;

public slots:
    void onVDChanged(unsigned int newVD);

    void onDealFunc(uint funcIndex);

private:
    MgsMainWindow *m_mainWindow = nullptr;
};
