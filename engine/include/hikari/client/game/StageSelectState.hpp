#ifndef HIKARI_CLIENT_GAME_STAGESELECTSTATE
#define HIKARI_CLIENT_GAME_STAGESELECTSTATE

#include "hikari/core/game/GameState.hpp"
#include "hikari/core/geom/Point2D.hpp"

#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/View.hpp>

#include <memory>

#include <json/value.h>

#include <string>

namespace sf {
    class Sprite;
}

namespace gcn {
    class Container;
    class Label;
    class Icon;
}

namespace hikari {

    class AudioService;
    class GameProgress;
    class GuiService;
    class ImageFont;
    class ServiceLocator;
    class GameController;

    namespace gui {
        // Forward-declare any GUI classes here
        class Icon;
    }

    class StageSelectState : public GameState {
    private:
        std::string name;
        GameController & controller;
        sf::View view;
        std::weak_ptr<GuiService> guiService;
        std::weak_ptr<AudioService> audioService;
        std::weak_ptr<GameProgress> gameProgress;
        std::unique_ptr<gcn::Container> guiContainer;
        std::unique_ptr<gcn::Label> guiSelectedCellLabel;
        std::unique_ptr<gcn::Icon> guiCursorIcon;
        std::unique_ptr<gui::Icon> guiForeground;
        std::unique_ptr<gui::Icon> guiBackground;
        std::unique_ptr<gui::Icon> guiLeftEye;
        std::unique_ptr<gui::Icon> guiRightEye;

        sf::Sprite background;
        sf::Sprite foreground;
        sf::Sprite leftEye;
        sf::Sprite rightEye;

        int cursorRow;
        int cursorColumn;
        int cursorIndex;
        bool startGamePlay;

        std::vector< std::pair< Point2D<float>, Point2D<float> > > eyePositions;
        std::vector< Point2D<float> > cursorPositions;

        static const char* PROPERTY_BACKGROUND;
        static const char* PROPERTY_FOREGROUND;
        static const char* PROPERTY_CURSOR_SPRITE;
        static const char* PROPERTY_CURSOR_POSITIONS;
        static const char* PROPERTY_EYE_SPRITE;
        static const char* PROPERTY_EYE_POSITIONS;
        static const char* PROPERTY_SPRITES;
        static const char* PROPERTY_X;
        static const char* PROPERTY_Y;
        static const char* PROPERTY_WIDTH;
        static const char* PROPERTY_HEIGHT;
        static const int NUM_OF_PORTRAITS;
        static const int NUM_OF_CURSOR_ROWS;
        static const int NUM_OF_CURSOR_COLUMNS;
        static const int DEFAULT_CURSOR_ROW;
        static const int DEFAULT_CURSOR_COLUMN;

        void calculateCursorIndex();
        void buildGui();

    public:
        StageSelectState(const std::string &name, const Json::Value &params, GameController & controller, ServiceLocator &services);
        virtual ~StageSelectState();

        virtual void handleEvent(sf::Event &event);
        virtual void render(sf::RenderTarget &target);
        virtual bool update(const float &dt);
        virtual void onEnter();
        virtual void onExit();
        virtual const std::string &getName() const;
    };

} // hikari

#endif // HIKARI_CLIENT_GAME_STAGESELECTSTATE
