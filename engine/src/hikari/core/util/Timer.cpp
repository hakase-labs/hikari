#include "hikari/core/util/Timer.hpp"
#include <cassert>

namespace hikari {

    const Timer::TimerCallback Timer::DEFAULT_CALLBACK = [](Timer &timer) { timer.stop(); };

    Timer::Timer()
        : running(false)
        , elapsed(0)
        , interval(1.0)
        , callback(DEFAULT_CALLBACK)
    {
        assert(interval != 0);
    }

    Timer::Timer(const float &interval)
        : running(false)
        , elapsed(0)
        , interval(interval)
        , callback(DEFAULT_CALLBACK)
    {
        assert(interval != 0);
    }

    Timer::Timer(const float &interval, const TimerCallback &callback)
        : running(false)
        , elapsed(0)
        , interval(interval)
        , callback(callback)
    {
        assert(interval != 0);
    }

    Timer& Timer::setCallback(const TimerCallback &callback) {
        this->callback = callback;
        return *this;
    }

    const Timer::TimerCallback& Timer::getCallback() const {
        return callback;
    }

    Timer& Timer::setInterval(const float &interval) {
        this->interval = interval;
        assert(interval != 0);
        return *this;
    }

    const float Timer::getInterval() const {
        return interval;
    }

    const float Timer::getElapsed() const {
        return elapsed;
    }

    Timer& Timer::reset() {
        elapsed = 0;
        return *this;
    }

    Timer& Timer::start() {
        running = true;
        return *this;
    }

    Timer& Timer::stop() {
        running = false;
        return *this;
    }

    const bool Timer::isRunning() const {
        return running;
    }

    void Timer::update(float dt) {
        if(isRunning()) {
            elapsed += dt;

            while(elapsed >= interval) {
                if(callback) {
                    callback(*this);
                }
                elapsed -= interval;
            }
        }
    }
}