#include "hikari/client/game/PasswordState.hpp"
#include "hikari/client/gui/Panel.hpp"
#include "hikari/core/util/ServiceLocator.hpp"

#include <guichan/gui.hpp>
#include <guichan/widgets/label.hpp>
#include <json/reader.h>

namespace hikari {

    PasswordState::PasswordState(const std::string &name, const Json::Value &params, ServiceLocator &services)
        : name(name)
    {

    }

    PasswordState::~PasswordState() {

    }

    void PasswordState::handleEvent(sf::Event &event) {

    }

    void PasswordState::render(sf::RenderTarget &target) {

    }

    bool PasswordState::update(const float &dt) {
        return false;
    }

    void PasswordState::onEnter() {

    }

    void PasswordState::onExit() {

    }

    const std::string & PasswordState::getName() const {
        return name;
    }

} // hikari