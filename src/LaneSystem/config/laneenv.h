#ifndef LANEENV_H
#define LANEENV_H

#include <QObject>

class LaneEnv : public QObject
{
    Q_OBJECT
public:
    explicit LaneEnv(QObject *parent = nullptr);

signals:
};

#endif // LANEENV_H
