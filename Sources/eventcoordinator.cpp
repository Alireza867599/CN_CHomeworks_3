#include "eventcoordinator.h"
#include <QDebug>

EventsCoordinator::EventsCoordinator(QThread *parent) :
    QThread {parent},
    m_timer {new QTimer(this)}
{
    connect(m_timer, &QTimer::timeout, this, &EventsCoordinator::handleTick);
}

EventsCoordinator *EventsCoordinator::instance(QThread *parent)
{
    if (!m_self)
    {
        m_self = new EventsCoordinator(parent);
    }
    return m_self;
}


void EventsCoordinator::release()
{
    if (m_self)
    {
        delete m_self;
        m_self = nullptr;
    }
}

void EventsCoordinator::startSimulation(int durationMs, DataGenerator* generator)
{
    if (m_timer->isActive())
    {
        qDebug() << "Simulation is already running!";
        return;
    }

    m_dataGenerator = generator;

    m_totalDuration = durationMs;
    m_elapsedTime = 0;

    m_timer->start(1000);
    emit simulationStarted();
    qDebug() << "Simulation started for" << durationMs << "milliseconds";
}

void EventsCoordinator::stopSimulation()
{
    if (m_timer->isActive())
    {
        m_timer->stop();
        emit simulationStopped();
        qDebug() << "Simulation stopped!";
    }
}

void EventsCoordinator::resetSimulation()
{
    stopSimulation();
    m_elapsedTime = 0;
    m_totalDuration = 0;
    m_dataGenerator = nullptr;
    qDebug() << "Simulation reset!";
}

void EventsCoordinator::handleTick()
{
    m_elapsedTime += 1000;

    if (m_dataGenerator) {
        double generatedData = m_dataGenerator->generate();
        emit dataGenerated(generatedData);
        qDebug() << "Generated data:" << generatedData;
    }

    emit tick();

    if (m_elapsedTime >= m_totalDuration)
    {
        stopSimulation();
    }
}
