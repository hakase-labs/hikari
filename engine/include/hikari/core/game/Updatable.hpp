#ifndef HIKARI_CLIENT_GAME_UPDATABLE
#define HIKARI_CLIENT_GAME_UPDATABLE

namespace hikari {
    
    /**
        Interface for all things that can be updated over time.
    */
    class Updatable {
    public:
        /**
            Virtual destructor; standard stuff.
        */
        virtual ~Updatable() {
            // no-op
        }

        /**
            Updates an object by a number of seconds.

            @param dt the number of seconds to update for
        */
        virtual void update(float dt) = 0;
    };

} // hikari

#endif // HIKARI_CLIENT_GAME_UPDATABLE