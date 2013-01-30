
#include "hikari/client/game/objects/effects/NothingEffect.hpp"
#include "hikari/core/util/Log.hpp"

namespace hikari {

    NothingEffect::NothingEffect() {

    }

    NothingEffect::NothingEffect(const NothingEffect &proto) {
        HIKARI_LOG(debug) << "NothingEffect copy constructor";
    }

    std::shared_ptr<Effect> NothingEffect::clone() const {
        return std::make_shared<NothingEffect>(*this);
    }

    NothingEffect::~NothingEffect() {

    }

    void NothingEffect::apply() {
        // Nothing to do here!
        HIKARI_LOG(info) << "NothingEffect::apply()";
    }

    void NothingEffect::unapply() {
        // Nothing to do here!
        HIKARI_LOG(info) << "NothingEffect::unapply()";
    }

} // hikari