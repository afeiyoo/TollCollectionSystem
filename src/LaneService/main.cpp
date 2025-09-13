#include "laneservice.h"

#include <QCoreApplication>

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    LaneService *server = new LaneService(nullptr);
    server->init();

    return a.exec();
}
