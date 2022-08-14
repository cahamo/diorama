#ifndef COUNTDOWNTIMER_H
#define COUNTDOWNTIMER_H

#include <Arduino.h>

// Default countdown time in milliseconds
#define COUNTDOWNTIMER_DEFAULT_TIME 1000

/*!
 * @brief Encapsulation of a basic countdowm timer.
 */
class CountdownTimer {
    public:
        /*!
         * @brief Instantiates a new coutdown timer that runs for the default time.
         */
        CountdownTimer();

        /*!
         * @brief Instantiates a new countdown timer that is to run for a specified time.
         *
         * The countdown does not start until the start() or reset() method is called.
         *
         * @param timeMS Time to countdown in milliseconds.
         */
        CountdownTimer(unsigned long timeMs);

        /*!
         * @brief Starts the countdown, or restarts it if active.
         */
        void start();

        /*!
         * @brief Restarts the countdown. Synonym for start().
         */
        void restart();

        /*!
         * @brief Stops any currently active countdown. Does nothing if the countdown is not running.
         */
        void stop();

        /*!
         * @brief Checks if the countdown is running.
         *
         * Note that a countdown may have timed out and still be running.
         *
         * @return True if the countdown is running or False if it is stopped.
         */
        bool isRunning() const;

        /*!
         * @brief Checks if a running countdown has timed out.
         *
         * @return True if the countdown has timed out, false otherwise. False is also returned if the countdown is not running.
         */
        bool hasTimedOut() const;

        /*!
         * @brief Gets the length of the current countdown.
         *
         * @return Current countdown length in milliseconds.
         */
        unsigned long getLengthMs() const;

        /*!
         * @brief Get time ellapsed since countdown started.
         *
         * @return Ellapsed time in milliseconds or countdown length if timed out. 0 is returned if countdown is not running.
         */
        unsigned long ellapsedTimeMs() const;

        /*!
         * @brief Get time remaining before countdown expires.
         *
         * @return Remaining time in milliseconds or 0 if countdown has timed out or is not running.
         */
        unsigned long remainingTimeMs() const;

    private:
        unsigned long _timeMs = COUNTDOWNTIMER_DEFAULT_TIME;
        unsigned long _startTime = 0;
};

#endif
