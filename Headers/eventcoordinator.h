#ifndef EVENTSCOORDINATOR_H
#define EVENTSCOORDINATOR_H

#include <chrono>
#include <QObject>
#include <QThread>
#include <QTimer>
#include "DataGenerator.h"

class EventsCoordinator : public QThread {
    Q_OBJECT

    typedef std::chrono::milliseconds Millis;

    explicit EventsCoordinator(QThread *parent = nullptr);

public:
    ~EventsCoordinator() override = default;
    static EventsCoordinator *instance(QThread *parent = nullptr);
    static void release();

    void startSimulation(int durationMs, DataGenerator* generator);
    void stopSimulation();
    void resetSimulation();

Q_SIGNALS:
    void tick();
    void simulationStarted();
    void simulationStopped();
    void dataGenerated(double value);

private Q_SLOTS:
    void handleTick();

private:
    inline static EventsCoordinator *m_self = nullptr;
    QTimer *m_timer = nullptr;
    DataGenerator* m_dataGenerator = nullptr;
    int m_totalDuration = 0;
    int m_elapsedTime = 0;
};

#endif // EVENTSCOORDINATOR_H

