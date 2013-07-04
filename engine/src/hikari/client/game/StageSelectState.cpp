#include "hikari/client/game/StageSelectState.hpp"
#include "hikari/client/audio/AudioService.hpp"
#include "hikari/client/game/GameProgress.hpp"
#include "hikari/client/gui/GuiService.hpp"
#include "hikari/client/Services.hpp"

#include "hikari/core/gui/ImageFont.hpp"
#include "hikari/core/util/ImageCache.hpp"
#include "hikari/core/util/StringUtils.hpp"
#include "hikari/core/util/ServiceLocator.hpp"
#include "hikari/core/util/Log.hpp"

#include <SFML/Graphics.hpp>
#include <SFML/Window/Event.hpp>

#include <guichan/widgets/container.hpp>
#include <guichan/widgets/label.hpp>

namespace hikari {

    //std::string name;
    //    sf::View view;
    ////    std::shared_ptr<GameProgress> gameProgress;
    //    std::shared_ptr<ImageFont> guiFont;
    const char* StageSelectState::PROPERTY_BACKGROUND = "background";
    const char* StageSelectState::PROPERTY_FOREGROUND = "foreground";
    const char* StageSelectState::PROPERTY_CURSOR_SPRITE = "cursor";
    const char* StageSelectState::PROPERTY_CURSOR_POSITIONS = "cursorPositions";
    const char* StageSelectState::PROPERTY_EYE_SPRITE = "eye";
    const char* StageSelectState::PROPERTY_EYE_POSITIONS = "eyePositions";
    const char* StageSelectState::PROPERTY_X = "x";
    const char* StageSelectState::PROPERTY_Y = "y";
    const char* StageSelectState::PROPERTY_WIDTH = "w";
    const char* StageSelectState::PROPERTY_HEIGHT = "h";
    const int StageSelectState::NUM_OF_PORTRAITS = 9;
    const int StageSelectState::NUM_OF_CURSOR_ROWS = 3;
    const int StageSelectState::NUM_OF_CURSOR_COLUMNS = 3;
    const int StageSelectState::DEFAULT_CURSOR_ROW = 1;
    const int StageSelectState::DEFAULT_CURSOR_COLUMN = 1;

    StageSelectState::StageSelectState(const std::string &name, const Json::Value &params, ServiceLocator &services)
        : name(name)
        , guiService(services.locateService<GuiService>(Services::GUISERVICE))
        , audioService(services.locateService<AudioService>(Services::AUDIO))
        , gameProgress(services.locateService<GameProgress>(Services::GAMEPROGRESS))
        , guiContainer(new gcn::Container())
        , guiSelectedCellLabel(new gcn::Label())
        , cursorRow(DEFAULT_CURSOR_ROW)
        , cursorColumn(DEFAULT_CURSOR_COLUMN)
    {
        std::weak_ptr<ImageCache> imageCache = services.locateService<ImageCache>(Services::IMAGECACHE);

        // Load sprites from config
        // TODO: This needs to be refactored to be safer and things like that
        // TODO: Need a utility method to load sf:Sprite from JSON
        if(auto imageCachePtr = imageCache.lock()) {
            background.setTexture(*imageCachePtr->get(params[PROPERTY_BACKGROUND].asString()).get());
            foreground.setTexture(*imageCachePtr->get(params[PROPERTY_FOREGROUND].asString()).get());
            cursor.setTexture(*imageCachePtr->get(params[PROPERTY_CURSOR_SPRITE].asString()).get());
            leftEye.setTexture(*imageCachePtr->get(params[PROPERTY_EYE_SPRITE].asString()).get());
            rightEye.setTexture(*imageCachePtr->get(params[PROPERTY_EYE_SPRITE].asString()).get());
        }

        cursor.setPosition(-100.0f, -100.0f);

        // Load eye positions
        // There are 9 positions; 18 points total (one for left, one for right eye)
        const Json::Value &jsonEyePositions = params[PROPERTY_EYE_POSITIONS];
        if(jsonEyePositions.size() == NUM_OF_PORTRAITS) {
            for(int i = 0; i < NUM_OF_PORTRAITS; ++i) {
                const Json::Value &jsonEyePosition = jsonEyePositions[i];
                float leftEyeX = static_cast<float>(jsonEyePosition[0u].get(PROPERTY_X, 0).asInt());
                float leftEyeY = static_cast<float>(jsonEyePosition[0u].get(PROPERTY_Y, 0).asInt());
                float rightEyeX = static_cast<float>(jsonEyePosition[1u].get(PROPERTY_X, 0).asInt());
                float rightEyeY = static_cast<float>(jsonEyePosition[1u].get(PROPERTY_Y, 0).asInt());

                Point2D<float> leftEyePosition(leftEyeX, leftEyeY);
                Point2D<float> rightEyePosition(rightEyeX, rightEyeY);

                eyePositions.push_back(std::make_pair(leftEyePosition, rightEyePosition));
            }
        }

        // Load cursor positions
        const Json::Value &jsonCursorPositions = params[PROPERTY_CURSOR_POSITIONS];
        if(jsonCursorPositions.size() == NUM_OF_PORTRAITS) {
            for(int i = 0; i < NUM_OF_PORTRAITS; ++i) {
                const Json::Value &jsonCursorPosition = jsonCursorPositions[i];
                float cursorX = static_cast<float>(jsonCursorPosition.get(PROPERTY_X, 0).asInt());
                float cursorY = static_cast<float>(jsonCursorPosition.get(PROPERTY_Y, 0).asInt());

                Point2D<float> position(cursorX, cursorY);

                cursorPositions.push_back(position);
            }
        }

        buildGui();
    }

    StageSelectState::~StageSelectState() {

    }

    void StageSelectState::calculateCursorIndex() {
        cursorIndex = (cursorRow * NUM_OF_CURSOR_ROWS) + cursorColumn;
    }

    void StageSelectState::buildGui() {
        guiContainer->setSize(256, 240);
        guiContainer->setBaseColor(0x1122AA);
        guiContainer->setOpaque(true);
        guiContainer->setVisible(true);

        guiSelectedCellLabel->setX(8);
        guiSelectedCellLabel->setY(224);
        guiSelectedCellLabel->setCaption("(" + StringUtils::toString(cursorColumn) + ", " + StringUtils::toString(cursorRow) + ")");
        guiSelectedCellLabel->adjustSize();

        guiContainer->add(guiSelectedCellLabel.get());
    }

    void StageSelectState::handleEvent(sf::Event &event) {
        if(event.type == sf::Event::KeyPressed) {
            if(event.key.code == sf::Keyboard::Up) {
                //gameProgress->setCurrentBoss(gameProgress->getCurrentBoss() + 1);
                cursorRow = std::max(0, cursorRow - 1);
            } else if(event.key.code == sf::Keyboard::Down) {
                //gameProgress->setCurrentBoss(gameProgress->getCurrentBoss() - 1);
                cursorRow = std::min(NUM_OF_CURSOR_ROWS - 1, cursorRow + 1);
            } else if(event.key.code == sf::Keyboard::Left) {
                cursorColumn = std::max(0, cursorColumn - 1);
            } else if(event.key.code == sf::Keyboard::Right) {
                cursorColumn = std::min(NUM_OF_CURSOR_COLUMNS - 1, cursorColumn + 1);
            }

            guiSelectedCellLabel->setCaption("(" + StringUtils::toString(cursorColumn) + ", " + StringUtils::toString(cursorRow) + ")");
            guiSelectedCellLabel->adjustSize();
        }
    }
    
    void StageSelectState::render(sf::RenderTarget &target) {
        //target.draw(background);
        target.draw(leftEye);
        target.draw(rightEye);
        target.draw(foreground);
        target.draw(cursor);
        
        // guiFont->renderText(target, "PUSH   START", 80, 8);
        // guiFont->renderText(target, "MAN", 48, 88);
        // guiFont->renderText(target, "MAN", 128, 88);
        // guiFont->renderText(target, "MAN", 208, 88);
        // guiFont->renderText(target, "MAN", 48, 152);
        // // guiFont->renderText(target, "MAN", 128, 152);
        // guiFont->renderText(target, "MAN", 208, 152);
        // guiFont->renderText(target, "MAN", 48, 216);
        // guiFont->renderText(target, "MAN", 128, 216);
        // guiFont->renderText(target, "MAN", 208, 216);

        // guiFont->renderText(target, "Boss: " + StringUtils::toString<int>(static_cast<int>(gameProgress->getCurrentBoss())), 8, 224);
        // guiFont->renderText(target, "(" + StringUtils::toString<int>(cursorColumn) + ", " + StringUtils::toString<int>(cursorRow) + ")", 8, 224);
    }

    bool StageSelectState::update(const float &dt) {
        calculateCursorIndex();

        // Set eye positions
        std::pair< Point2D<float>, Point2D<float> > eyePosition = eyePositions.at(cursorIndex);

        const Point2D<float> &leftEyePosition = eyePosition.first;
        leftEye.setPosition(leftEyePosition.getX(), leftEyePosition.getY());

        const Point2D<float> &rightEyePosition = eyePosition.second;
        rightEye.setPosition(rightEyePosition.getX(), rightEyePosition.getY());

        // Set cursor position
        const Point2D<float> &cursorPosition = cursorPositions.at(cursorIndex);
        cursor.setPosition(cursorPosition.getX(), cursorPosition.getY());

        return false;
    }

    void StageSelectState::onEnter() {
        // Start music
        if(auto audio = audioService.lock()) {
            audio->playMusic(3);
        }

        // Reset cursor to default location
        cursorColumn = DEFAULT_CURSOR_COLUMN;
        cursorRow = DEFAULT_CURSOR_ROW;

        // Attach our GUI
        if(auto gui = guiService.lock()) {
            auto & topContainer = gui->getRootContainer();

            topContainer.add(guiContainer.get(), 0, 0);
        }
    }

    void StageSelectState::onExit() {
        // Stop music
        if(auto audio = audioService.lock()) {
            audio->stopMusic();
        }

        // Remove our GUI
        if(auto gui = guiService.lock()) {
            auto & topContainer = gui->getRootContainer();

            topContainer.remove(guiContainer.get());
        }
    }

    const std::string& StageSelectState::getName() const {
        return name;
    }

} // hikari