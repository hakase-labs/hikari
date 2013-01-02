#ifndef HIKARI_CLIENT_GAME_GAMEOBJECT
#define HIKARI_CLIENT_GAME_GAMEOBJECT

#include <hikari/core/game/Updatable.hpp>
#include <functional>
// #include <oolua.h>

namespace hikari {
    
    /**

    */
    class GameObject : public Updatable {
    public:
        //typedef std::function<void (GameObject&, const float &)> Brain;
        //Brain brain;

        /**

        */
        static const int  generateObjectId();

    private:
        int id;
        bool active;
        static int nextId;

    public:
        explicit GameObject(const int& id);
        virtual ~GameObject();

        int getId() const;
        
        const bool isActive() const;
        void setActive(const bool& active);

        virtual void update(const float &dt);

        virtual void reset() {};
    };

} // hikari

//
// Script binding delcarations
//
// namespace OOLUA {
    // typedef hikari::GameObject GameObject;
// }

//OOLUA_CLASS_NO_BASES(GameObject)
//    OOLUA_TYPEDEFS Abstract OOLUA_END_TYPES
//    OOLUA_MEM_FUNC_0_CONST(int, getId)
//OOLUA_CLASS_END

/*
//#include <hikari/core/math/FixedPoint.hpp>
#include <hikari/client/game/GOCreationParameters.hpp>
#include <hikari/client/game/Direction.hpp>
#include <hikari/core/geom/Rectangle2D.hpp>
#include <hikari/core/geom/Point2D.hpp>

#include <functional>
#include <oolua.h>

namespace sf {
    class RenderTarget;
}

namespace hikari {

    class GameObject {
    public:
        typedef std::function<void (GameObject&, const float &)> Brain;

    private:
        int id;
        int state;
        int health;
        int maxHealth;
        
        float age;
        
        bool active;
        bool affectedByGravity;

        Point2D<float> position;
        Point2D<float> spawnPosition;
        Point2D<float> velocity;

        Directions::Enum direction;

        Brain brain;

        struct DefaultBrain {
            void operator() (GameObject &object, const float &dt);
        };

        GOCreationParameters creationParams;

        // Animation pointer

    protected:
        Rectangle2D<float> boundingBox;

        void setState(const int &state);
        void setHealth(const int &health);
        void setMaxHealth(const int &health);
        void setActive(const bool &active);
        void setGravitated(const bool &affectedByGravity);
        void setDirection(const Directions::Enum &direction);

    public:
        GameObject(const int &id);
        GameObject(const int &id, const GOCreationParameters &params);
        virtual ~GameObject() { }

        int getId() const;
        const int &getState() const;
        const int &getHealth() const;
        const int &getMaxHealth() const;

        const float &getAge() const;

        const bool &isActive() const;
        const bool &isGravitated() const;

        const Point2D<float> &getPosition() const;
        const Point2D<float> &getSpawnPosition() const;
        const Point2D<float> &getVelocity() const;

        const Rectangle2D<float> &getBoundingBox() const;

        const Directions::Enum &getDirection() const;

        const Brain &getBrain() const;
        void setBrain(const Brain &brain);
        void setSpawnPosition(const Point2D<float> &spawnPosition);
        
        virtual void render(sf::RenderTarget &target);
        virtual void update(const float &dt);
    };

}

//
// Script binding delcarations
//
namespace OOLUA {
    typedef hikari::GameObject GameObject;
}

OOLUA_CLASS_NO_BASES(GameObject)
    OOLUA_TYPEDEFS No_public_constructors OOLUA_END_TYPES
    OOLUA_MEM_FUNC_0_CONST(int, getId)
OOLUA_CLASS_END

*/

#endif // HIKARI_CLIENT_GAME_GAMEOBJECT