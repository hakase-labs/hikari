#ifndef HIKARI_CLIENT_GAME_GAMEOBJECT
#define HIKARI_CLIENT_GAME_GAMEOBJECT

#include "hikari/core/game/Updatable.hpp"

#include <functional>

namespace hikari {
    
    class GameObject : public Updatable {
    //
    // Static members
    //
    public:
        static const int  generateObjectId();

    private:
        static int nextId;
    
    //
    // Class members
    //
    private:
        int id;
        bool active;

    protected:
        virtual void onActivated();
        virtual void onDeactivated();

    public:
        explicit GameObject(int id = generateObjectId());
        virtual ~GameObject();

        int getId() const;
        
        bool isActive() const;
        void setActive(bool active);

        virtual void update(float dt);

        virtual void reset() {};
    };

} // hikari

#endif // HIKARI_CLIENT_GAME_GAMEOBJECT