#include "hikari/client/game/StageSelectState.hpp"
#include "hikari/client/audio/AudioService.hpp"
#include "hikari/client/game/GameProgress.hpp"
#include "hikari/client/game/Task.hpp"
#include "hikari/client/game/FunctionTask.hpp"
#include "hikari/client/game/WaitTask.hpp"
#include "hikari/client/gui/GuiService.hpp"
#include "hikari/client/gui/Icon.hpp"
#include "hikari/client/Services.hpp"

#include "hikari/core/game/GameController.hpp"
#include "hikari/core/gui/ImageFont.hpp"
#include "hikari/core/util/ImageCache.hpp"
#include "hikari/core/util/StringUtils.hpp"
#include "hikari/core/util/ServiceLocator.hpp"
#include "hikari/core/util/Log.hpp"

#include <SFML/Graphics.hpp>
#include <SFML/Window/Event.hpp>

#include <guichan/widgets/container.hpp>
#include <guichan/widgets/label.hpp>
#include <guichan/widgets/icon.hpp>

namespace hikari {

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

    StageSelectState::StageSelectState(const std::string &name, const Json::Value &params, GameController & controller, ServiceLocator &services)
        : name(name)
        , controller(controller)
        , guiService(services.locateService<GuiService>(Services::GUISERVICE))
        , audioService(services.locateService<AudioService>(Services::AUDIO))
        , gameProgress(services.locateService<GameProgress>(Services::GAMEPROGRESS))
        , taskQueue()
        , guiContainer(new gcn::Container())
        , guiSelectedCellLabel(new gcn::Label())
        , guiCursorIcon()
        , guiForeground()
        , guiBackground()
        , guiLeftEye()
        , guiRightEye()
        , cursorRow(DEFAULT_CURSOR_ROW)
        , cursorColumn(DEFAULT_CURSOR_COLUMN)
        , enableCursorMovement(false)
    {
        std::weak_ptr<ImageCache> imageCache = services.locateService<ImageCache>(Services::IMAGECACHE);

        // Load sprites from config
        // TODO: This needs to be refactored to be safer and things like that
        // TODO: Need a utility method to load sf:Sprite from JSON
        if(auto imageCachePtr = imageCache.lock()) {
            background.setTexture(*imageCachePtr->get(params[PROPERTY_BACKGROUND].asString()).get());
            foreground.setTexture(*imageCachePtr->get(params[PROPERTY_FOREGROUND].asString()).get());
            leftEye.setTexture(*imageCachePtr->get(params[PROPERTY_EYE_SPRITE].asString()).get());
            rightEye.setTexture(*imageCachePtr->get(params[PROPERTY_EYE_SPRITE].asString()).get());
        }

        guiCursorIcon.reset(new gcn::Icon(params[PROPERTY_CURSOR_SPRITE].asString()));

        guiCursorIcon->setX(-100);
        guiCursorIcon->setY(-100);

        // Load eye positions
        // There are 9 positions; 18 points total (one for left, one for right eye)
        const Json::Value &jsonEyePositions = params[PROPERTY_EYE_POSITIONS];
        if(static_cast<int>(jsonEyePositions.size()) == NUM_OF_PORTRAITS) {
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
        if(static_cast<int>(jsonCursorPositions.size()) == NUM_OF_PORTRAITS) {
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

    void StageSelectState::selectCurrentPortrait() {
        calculateCursorIndex();

        // Set eye positions
        std::pair< Point2D<float>, Point2D<float> > eyePosition = eyePositions.at(cursorIndex);

        const Point2D<float> &leftEyePosition = eyePosition.first;
        leftEye.setPosition(leftEyePosition.getX(), leftEyePosition.getY());
        guiLeftEye->setX(static_cast<int>(leftEyePosition.getX()));
        guiLeftEye->setY(static_cast<int>(leftEyePosition.getY()));

        const Point2D<float> &rightEyePosition = eyePosition.second;
        rightEye.setPosition(rightEyePosition.getX(), rightEyePosition.getY());
        guiRightEye->setX(static_cast<int>(rightEyePosition.getX()));
        guiRightEye->setY(static_cast<int>(rightEyePosition.getY()));

        // Set cursor position
        const Point2D<float> &cursorPosition = cursorPositions.at(cursorIndex);

        guiCursorIcon->setX(cursorPosition.getX());
        guiCursorIcon->setY(cursorPosition.getY());
    }

    void StageSelectState::calculateCursorIndex() {
        cursorIndex = (cursorRow * NUM_OF_CURSOR_ROWS) + cursorColumn;
    }

    void StageSelectState::buildGui() {
        guiContainer->setSize(256, 240);
        guiContainer->setBaseColor(0x1f22AA);
        guiContainer->setOpaque(false);
        guiContainer->setVisible(true);

        guiSelectedCellLabel->setX(8);
        guiSelectedCellLabel->setY(224);
        guiSelectedCellLabel->setCaption("(" + StringUtils::toString(cursorColumn) + ", " + StringUtils::toString(cursorRow) + ")");
        guiSelectedCellLabel->adjustSize();

        guiBackground.reset(new gui::Icon("assets/images/bg-stage-select.png"));
        guiForeground.reset(new gui::Icon("assets/images/fg-stage-select.png"));

        guiLeftEye.reset(new gui::Icon("assets/images/eye-stage-select.png"));
        guiRightEye.reset(new gui::Icon("assets/images/eye-stage-select.png"));

        guiContainer->add(guiBackground.get());
        guiContainer->add(guiForeground.get());
        guiContainer->add(guiLeftEye.get());
        guiContainer->add(guiRightEye.get());
        guiContainer->add(guiSelectedCellLabel.get());
        guiContainer->add(guiCursorIcon.get());
    }

    void StageSelectState::handleEvent(sf::Event &event) {
        bool playSample = false;

        if(event.type == sf::Event::KeyPressed) {
            if(enableCursorMovement) {
                if(event.key.code == sf::Keyboard::Up) {
                    cursorRow = std::max(0, cursorRow - 1);
                    playSample = true;
                } else if(event.key.code == sf::Keyboard::Down) {
                    cursorRow = std::min(NUM_OF_CURSOR_ROWS - 1, cursorRow + 1);
                    playSample = true;
                } else if(event.key.code == sf::Keyboard::Left) {
                    cursorColumn = std::max(0, cursorColumn - 1);
                    playSample = true;
                } else if(event.key.code == sf::Keyboard::Right) {
                    cursorColumn = std::min(NUM_OF_CURSOR_COLUMNS - 1, cursorColumn + 1);
                    playSample = true;
                } else if(event.key.code == sf::Keyboard::Return) {
                    // Play the "selected" sound
                    taskQueue.push(std::make_shared<FunctionTask>(0, [&](float dt) -> bool {
                        if(auto audio = audioService.lock()) {
                            audio->playSample("Stage Selected");
                        }

                        enableCursorMovement = false;

                        return true;
                    }));

                    // Wait half a second before continuing
                    taskQueue.push(std::make_shared<WaitTask>(0.5f));

                    // Stop the regular music, start playing the boss intro music
                    taskQueue.push(std::make_shared<FunctionTask>(0, [&](float dt) -> bool {
                        if(auto audio = audioService.lock()) {
                            audio->stopMusic();
                            audio->playMusic("Boss Selected (MM3)");
                        }

                        return true;
                    }));

                    // Wait 7 seconds for the music to play
                    taskQueue.push(std::make_shared<WaitTask>(7.0f));

                    // Go to the next game state -- playing the game
                    taskQueue.push(std::make_shared<FunctionTask>(0, [&](float dt) -> bool {
                        controller.requestStateChange("gameplay");
                        startGamePlay = true;

                        return true;
                    }));
                }

                selectCurrentPortrait();

                if(auto gp = gameProgress.lock()) {
                    gp->setCurrentBoss(cursorIndex);
                }

                if(playSample) {
                    if(auto audio = audioService.lock()) {
                        audio->playSample("Menu Item Select");
                    }
                }

                guiSelectedCellLabel->setCaption("(" + StringUtils::toString(cursorColumn) + ", " + StringUtils::toString(cursorRow) + ")");
                guiSelectedCellLabel->adjustSize();
            }
        }
    }

    void StageSelectState::render(sf::RenderTarget &target) {
        if(auto gui = guiService.lock()) {
            gui->renderAsTop(guiContainer.get(), target);
        }
        //target.draw(background);
        //target.draw(leftEye);
        //target.draw(rightEye);
        //target.draw(foreground);

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

    bool StageSelectState::update(float dt) {
        if(!taskQueue.empty()) {
            auto & task = taskQueue.front();
            task->update(dt);

            if(task->isComplete()) {
                taskQueue.pop();
            }
        } else {
            // calculateCursorIndex();
            selectCurrentPortrait();
        }

        return startGamePlay;
    }

    void StageSelectState::onEnter() {
        startGamePlay = false;
        enableCursorMovement = true;

        // Start music
        if(auto audio = audioService.lock()) {
            audio->playMusic("Stage Select (MM2)");
        }

        // Reset cursor to default location
        cursorColumn = DEFAULT_CURSOR_COLUMN;
        cursorRow = DEFAULT_CURSOR_ROW;

        if(auto gui = guiService.lock()) {
            auto & topContainer = gui->getRootContainer();

            topContainer.add(guiContainer.get(), 0, 0);
            guiContainer->setEnabled(true);
        }

        selectCurrentPortrait();
    }

    void StageSelectState::onExit() {
        enableCursorMovement = false;

        // Stop music
        if(auto audio = audioService.lock()) {
            audio->stopMusic();
        }

        // Remove our GUI
        if(auto gui = guiService.lock()) {
            auto & topContainer = gui->getRootContainer();

            topContainer.remove(guiContainer.get());
            guiContainer->setEnabled(false);
        }
    }

    const std::string& StageSelectState::getName() const {
        return name;
    }

} // hikari
