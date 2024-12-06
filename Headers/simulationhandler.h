#ifndef SIMULATIONHANDLER_H
#define SIMULATIONHANDLER_H

#include <QObject>
#include <QDebug>

class SimulationHandler : public QObject {
    Q_OBJECT
public:
    explicit SimulationHandler(QObject *parent = nullptr);

public slots:
    void onTick();
    void onSimulationStarted();
    void onSimulationStopped();
    void onDataGenerated(double value);
};

#endif // SIMULATIONHANDLER_H
