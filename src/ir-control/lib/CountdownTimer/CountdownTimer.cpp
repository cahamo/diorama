#include <Arduino.h>
#include "CountdownTimer.h"

CountdownTimer::CountdownTimer() {}

CountdownTimer::CountdownTimer(unsigned long timeMs) : _timeMs(timeMs) {}

void CountdownTimer::start() {
    _startTime = millis();
}

void CountdownTimer::restart() {
    start();
}

void CountdownTimer::stop() {
    _startTime = 0;
}

bool CountdownTimer::isRunning() const {
    return _startTime > 0;
}

bool CountdownTimer::hasTimedOut() const {
    if (!isRunning()) {
        return false;
    }
    return (millis() - _startTime) >= _timeMs;
}

unsigned long CountdownTimer::ellapsedTimeMs() const {
    if (!isRunning())
        return 0;   // not active - no time ellapsed
    unsigned long now = millis();
    if (now - _startTime >= _timeMs)
        return _timeMs;   // timed out - return timer length
    return now - _startTime;
}

unsigned long CountdownTimer::remainingTimeMs() const {
    if (!isRunning())
        return 0;   // not active - no time remaining
    return _timeMs - ellapsedTimeMs();    // will be zero if timed out
}

unsigned long CountdownTimer::getLengthMs() const {
    return _timeMs;
}
