#ifndef HIKARI_CORE_GAME_MAP_DOOR
#define HIKARI_CORE_GAME_MAP_DOOR

#include "hikari/core/Platform.hpp"
#include "hikari/core/game/Updatable.hpp"
#include "hikari/core/geom/BoundingBox.hpp"
#include "hikari/client/game/objects/animatedSprite.hpp"

#include <memory>

namespace sf {
    class RenderTarget;
}

namespace hikari {

    class HIKARI_API Door : public Updatable {
    private:
        enum DoorState {
            DOOR_CLOSED,
            DOOR_OPENING,
            DOOR_OPEN,
            DOOR_CLOSING
        };

    private:
        static const float DOOR_SECTION_DELAY_SECONDS;
        static const int DOOR_SECTION_COUNT;
        float timer;
        BoundingBox<int> bounds;
        AnimatedSprite animatedSprite;
        DoorState doorState;


    public:
        Door(int x = 0, int y = 0, int width = 1, int height = 3);
        virtual ~Door();

        int getX() const;
        int getY() const;

        int getWidth() const;
        int getHeight() const;

        bool isOpen() const;
        bool isClosed() const;

        // Copied from Entity; need to find a way to share this code
        // TODO: Refactor this so that Doors can share common code with Entity
        void setAnimationSet(const std::shared_ptr<AnimationSet> & newAnimationSet);
        void changeAnimation(const std::string& animationName);

        void open();
        void close();

        void setOpen();
        void setClosed();

        // Inherited from hikari::Updatable
        virtual void update(float dt);

        void render(sf::RenderTarget & target) const;
    };

} // hikari

#endif // HIKARI_CORE_GAME_MAP_DOOR