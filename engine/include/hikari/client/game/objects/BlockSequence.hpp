#ifndef HIKARI_CLIENT_GAME_BLOCKSEQUENCE
#define HIKARI_CLIENT_GAME_BLOCKSEQUENCE

#include "hikari/client/game/objects/BlockSequenceDescriptor.hpp"
#include "hikari/client/game/objects/GameObject.hpp"
#include "hikari/core/game/Renderable.hpp"

namespace hikari {   
    /**
     * A composite object that controls a sequence of appearing and
     * disappearing blocks. An instance of this class encapsulates the
     * operation of a single sequence/pattern of blocks.
     */
    class BlockSequence : public GameObject, public Renderable {
    private:
        BlockSequenceDescriptor descriptor;

    protected:
        virtual void onActivated();
        virtual void onDeactivated();

    public:
        explicit BlockSequence(const BlockSequenceDescriptor & descriptor, int id = GameObject::generateObjectId());
        virtual ~BlockSequence();

        virtual void update(float dt);

        virtual void reset();

        virtual void render(sf::RenderTarget &target);
    };

} // hikari

#endif // HIKARI_CLIENT_GAME_BLOCKSEQUENCE