#ifndef HIKARI_CLIENT_GAME_OBJECTS_EFFECTS_NOTHINGEFFECT
#define HIKARI_CLIENT_GAME_OBJECTS_EFFECTS_NOTHINGEFFECT

#include <hikari/client/game/Effect.hpp>
#include <memory>

namespace hikari {

    class NothingEffect : public Effect {
    public:
        NothingEffect();
        NothingEffect(const NothingEffect &proto);
        virtual std::shared_ptr<Effect> clone() const;
        virtual ~NothingEffect();
        virtual void apply();
        virtual void unapply();
    };

} // hikari

#endif // HIKARI_CLIENT_GAME_OBJECTS_EFFECTS_NOTHINGEFFECT