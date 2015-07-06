#ifndef HIKARI_CLIENT_GAME_BLOCKSEQUENCE
#define HIKARI_CLIENT_GAME_BLOCKSEQUENCE

#include "hikari/client/game/objects/BlockSequenceDescriptor.hpp"
#include "hikari/client/game/objects/GameObject.hpp"
#include "hikari/core/game/Renderable.hpp"
#include "hikari/core/geom/Rectangle2D.hpp"

#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/RectangleShape.hpp>

#include <memory>

namespace hikari {

    class EventBus;
    class Enemy;
    class GameWorld;

    /**
     * A composite object that controls a sequence of appearing and
     * disappearing blocks. An instance of this class encapsulates the
     * operation of a single sequence/pattern of blocks.
     */
    class BlockSequence : public GameObject, public Renderable {
    private:
        int zIndex;
        unsigned int step;
        float timer;
        BlockSequenceDescriptor descriptor;
        GameWorld & world;
        std::weak_ptr<EventBus> eventBus;
        std::vector<std::shared_ptr<Enemy>> blockEntities;
        sf::RectangleShape outlineShape;
        std::vector<sf::RectangleShape> blockRects;

    protected:
        virtual void onActivated();
        virtual void onDeactivated();

    public:
        BlockSequence(const BlockSequenceDescriptor & descriptor, GameWorld & world, int id = GameObject::generateObjectId());
        virtual ~BlockSequence();

        virtual void update(float dt);

        virtual void reset();

        virtual void render(sf::RenderTarget &target);
        virtual void setZIndex(int index);
        virtual int getZIndex() const;

        const Rectangle2D<int> & getBounds() const;
        void setEventBus(const std::weak_ptr<EventBus> & eventBus);
    };

} // hikari

#endif // HIKARI_CLIENT_GAME_BLOCKSEQUENCE