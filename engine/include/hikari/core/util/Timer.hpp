#ifndef HIKARI_CORE_UTIL_TIMER
#define HIKARI_CORE_UTIL_TIMER

#include "hikari/core/Platform.hpp"
#include "hikari/core/game/Updatable.hpp"
#include <functional>

namespace hikari {

    class Timer : public Updatable {
    public:
        typedef std::function<void (Timer &)> TimerCallback;

    private:
        bool running;
        float elapsed;
        float interval;
        TimerCallback callback;
        static const TimerCallback DEFAULT_CALLBACK;

    public:
        explicit Timer();
        explicit Timer(const float &interval);
        explicit Timer(const float &interval, const TimerCallback &callback);

        virtual ~Timer() { }

        Timer& setCallback(const TimerCallback &callback);
        const TimerCallback& getCallback() const;

        Timer& setInterval(const float &interval);
        const float getInterval() const;
        const float getElapsed() const;

        Timer& reset();
        Timer& start();
        Timer& stop();

        const bool isRunning() const;

        virtual void update(float dt);
    };

} // hikari

#endif // HIKARI_CORE_UTIL_TIMER
