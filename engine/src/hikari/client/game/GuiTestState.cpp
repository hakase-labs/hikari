#include <hikari/client/game/GuiTestState.hpp>
#include <hikari/core/util/PhysFS.hpp>
#include <hikari/core/util/PhysFSUtils.hpp>
#include <hikari/core/util/StringUtils.hpp>
#include <SFML/Window/Event.hpp>
#include <iostream>

namespace hikari {

    GuiTestState::GuiTestState(const std::string &name, const std::shared_ptr<hikari::ImageFont> &font)
        : name(name)
        , font(font) {

            PhysFSUtils::loadImage("assets/images/health-meter.png", energyMeterImage);
            energyMeterImage.setSmooth(false);

            sf::Sprite meterSprite(energyMeterImage);
            meterSprite.setTextureRect(sf::IntRect(0, 0, 8, 56));

            energyMeter.reset(new hikari::EnergyMeter(meterSprite, 56.0f));
            energyMeter->setValue(54.0f);
            energyMeter->setFillColor(sf::Color::Black);
            energyMeter->setPosition(sf::Vector2i(80, 100));
    }

    void GuiTestState::handleEvent(sf::Event &event) {
        if(event.type == sf::Event::KeyPressed) {
            if(event.key.code == sf::Keyboard::Up) {
                energyMeter->setValue(energyMeter->getValue() + 1);
            }
            if(event.key.code == sf::Keyboard::Down) {
                energyMeter->setValue(energyMeter->getValue() - 1);
            }
            if(event.key.code == sf::Keyboard::Left) {
                energyMeter->setOrientation(EnergyMeter::VERTICAL_ORIENTATION);
            }
            if(event.key.code == sf::Keyboard::Right) {
                energyMeter->setOrientation(EnergyMeter::HORIZONTAL_ORIENTATION);
            }
        }
    }

    void GuiTestState::render(sf::RenderTarget &target) {
        font->renderText(target, "TEST", 10, 10);
        font->renderText(target, "UPS: ", 10, 20);
        font->renderText(target, StringUtils::toString<float>(ups), 50, 20);
        font->renderText(target, "Missing CHARS?", 10, 30);

        font->renderText(target, StringUtils::toString<float>(energyMeter->getValue()), 100, 45);

        energyMeter->render(target);
    }

    bool GuiTestState::update(const float &dt) {
#ifdef _DEBUG
        //std::cout << "GuiTestState::update(" << dt << ")" << std::endl;
#endif
        ups = 1.0f / dt;
        return false;
    }

    void GuiTestState::onEnter() {
#ifdef _DEBUG
        std::cout << "GuiTestState::onEnter() called." << std::endl;
#endif
    }

    void GuiTestState::onExit() {
#ifdef _DEBUG
        std::cout << "GuiTestState::onExit() called." << std::endl;
#endif
    }

    const std::string& GuiTestState::getName() const {
        return name;
    }

} // hikari