#ifndef HIKARI_CLIENT_GAME_EFFECT
#define HIKARI_CLIENT_GAME_EFFECT

#include <hikari/core/util/Cloneable.hpp>
#include <memory>

namespace hikari {

    /**
        Interface for game propery changing effects (extra life, weapon energy, etc.)
    */
    class Effect {
    public:
        virtual std::shared_ptr<Effect> clone() const = 0;
        virtual ~Effect();
        virtual void apply() = 0;
        virtual void unapply() = 0;
    };

} // hikari

#endif // HIKARI_CLIENT_GAME_EFFECT