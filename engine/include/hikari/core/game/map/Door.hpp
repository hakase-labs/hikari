#ifndef HIKARI_CORE_GAME_MAP_DOOR
#define HIKARI_CORE_GAME_MAP_DOOR

#include "hikari/core/Platform.hpp"
#include "hikari/core/game/Updatable.hpp"
#include "hikari/core/geom/BoundingBox.hpp"

namespace hikari {

    class HIKARI_API Door : public Updatable {
    private:
        bool openFlag;
        bool closedFlag;
        BoundingBox<int> bounds;

    public:
        Door(int x = 0, int y = 0, int width = 1, int height = 3);
        virtual ~Door();

        int getX() const;
        int getY() const;

        int getWidth() const;
        int getHeight() const;

        void open();
        void close();

        void setOpen();
        void setClosed();

        // Inherited from hikari::Updatable
        virtual void update(float dt);
    };

} // hikari

#endif // HIKARI_CORE_GAME_MAP_DOOR