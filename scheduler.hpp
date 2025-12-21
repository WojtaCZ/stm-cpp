#ifndef STMCPP_SCHEDULER_H
#define STMCPP_SCHEDULER_H

#include <functional>
#include <stmcpp/units.hpp>
#include <stmcpp/systick.hpp>

namespace stmcpp::scheduler {
    using namespace stmcpp::units;

    /**
     * @brief A task scheduler that supports periodic execution, pausing, and resuming.
     */
    class Scheduler {
    public:
        using duration = stmcpp::units::duration;

        /**
         * @brief Construct a new Scheduler object
         * @param interval The time to wait before execution.
         * @param callback The function to call.
         * @param periodic If true, the task repeats automatically.
         * @param startImmediately If true, the timer starts counting upon construction.
         */
        Scheduler(duration interval, std::function<void()> callback, bool periodic = false, bool startImmediately = false) 
            : interval_(interval), callback_(callback), periodic_(periodic), active_(startImmediately), paused_(false) 
        {
            if (startImmediately) {
                lastRunTime_ = stmcpp::systick::getDuration();
            }
        }

        /**
         * @brief Check time and execute callback if due.
         * Must be called frequently in the main loop.
         */
        void dispatch() {
            // Do nothing if stopped or paused
            if (!active_ || paused_ || !callback_) {
                return;
            }

            duration now = stmcpp::systick::getDuration();

            // Check if interval has passed since the last run (or start)
            if ((now - lastRunTime_) >= interval_) {
                
                // Execute callback
                callback_();

                if (periodic_) {
                    // Reset reference time to now to start waiting for the next interval
                    lastRunTime_ = now;
                } else {
                    // If not periodic, stop the scheduler
                    active_ = false;
                }
            }
        }

        /**
         * @brief Starts the scheduler from scratch.
         * Resets the timer.
         */
        void start() {
            active_ = true;
            paused_ = false;
            lastRunTime_ = stmcpp::systick::getDuration();
        }

        /**
         * @brief Stops the scheduler completely.
         */
        void stop() {
            active_ = false;
            paused_ = false;
        }

        /**
         * @brief Pauses the timer.
         * The elapsed time is frozen until resume() is called.
         */
        void pause() {
            if (!active_ || paused_) return;
            
            paused_ = true;
            // Capture exactly when we paused
            pauseSnapshot_ = stmcpp::systick::getDuration();
        }

        /**
         * @brief Resumes the timer.
         * It continues counting from exactly where it left off.
         */
        void resume() {
            if (!active_ || !paused_) return;

            duration now = stmcpp::systick::getDuration();
            duration timeSpentPaused = now - pauseSnapshot_;

            // Shift the reference time forward by the amount of time we spent paused.
            // This effectively "deletes" the pause duration from the elapsed calculation.
            lastRunTime_ += timeSpentPaused;

            paused_ = false;
        }

        /**
         * @brief Manually reset the timer without triggering the callback.
         */
        void reset() {
            lastRunTime_ = stmcpp::systick::getDuration();
            // If we were paused, we need to update the snapshot too, 
            // otherwise resume() would calculate a huge jump.
            if (paused_) {
                pauseSnapshot_ = lastRunTime_;
            }
        }

        // --- Getters and Setters ---

        void setInterval(duration interval) {
            interval_ = interval;
        }

        void setPeriodic(bool periodic) {
            periodic_ = periodic;
        }

        bool isRunning() const {
            return active_ && !paused_;
        }

        bool isPaused() const {
            return paused_;
        }

    private:
        duration interval_;
        std::function<void()> callback_;
        bool periodic_;
        
        bool active_;
        bool paused_;

        duration lastRunTime_ = 0_ms;   // Reference point for interval calculation
        duration pauseSnapshot_ = 0_ms; // Stores timestamp when pause() was called
    };
}

#endif