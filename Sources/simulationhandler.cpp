#include "simulationhandler.h"

SimulationHandler::SimulationHandler(QObject *parent)
    : QObject(parent) {
}

void SimulationHandler::onTick() {
    qDebug() << "Tick received.";
}

void SimulationHandler::onSimulationStarted() {
    qDebug() << "Simulation started!";
}

void SimulationHandler::onSimulationStopped() {
    qDebug() << "Simulation stopped.";
}

void SimulationHandler::onDataGenerated(double value) {
    qDebug() << "Generated data:" << value;
}
