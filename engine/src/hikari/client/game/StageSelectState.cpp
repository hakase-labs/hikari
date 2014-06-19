#include "hikari/client/game/StageSelectState.hpp"
#include "hikari/client/audio/AudioService.hpp"
#include "hikari/client/game/GameProgress.hpp"
#include "hikari/client/game/Task.hpp"
#include "hikari/client/game/FunctionTask.hpp"
#include "hikari/client/game/WaitTask.hpp"
#include "hikari/client/gui/GuiService.hpp"
#include "hikari/client/gui/Icon.hpp"
#include "hikari/client/gui/IconAnimator.hpp"
#include "hikari/client/Services.hpp"

#include "hikari/core/game/GameController.hpp"
#include "hikari/core/gui/ImageFont.hpp"
#include "hikari/core/util/ImageCache.hpp"
#include "hikari/core/util/AnimationSetCache.hpp"
#include "hikari/core/util/StringUtils.hpp"
#include "hikari/core/util/ServiceLocator.hpp"
#include "hikari/core/util/Log.hpp"

#include <SFML/Graphics.hpp>
#include <SFML/Window/Event.hpp>

#include <guichan/widgets/container.hpp>
#include <guichan/widgets/label.hpp>
#include <guichan/widgets/icon.hpp>
#include <guichan/hakase/labelex.hpp>

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

    StageSelectState::StageSelectState(const std::string &name, const Json::Value &params, const StageSelectStateConfig & config, GameController & controller, ServiceLocator &services)
        : name(name)
        , controller(controller)
        , config(config)
        , guiService(services.locateService<GuiService>(Services::GUISERVICE))
        , audioService(services.locateService<AudioService>(Services::AUDIO))
        , gameProgress(services.locateService<GameProgress>(Services::GAMEPROGRESS))
        , taskQueue()
        , guiContainer(new gcn::Container())
        , guiFlashLayer(new gcn::Container())
        , guiBossIntroLayer(new gcn::Container())
        , guiBossStripe(new gcn::Container())
        , guiBossIntroLabel(new gcn::LabelEx())
        , guiSelectedCellLabel(new gcn::LabelEx())
        , guiForeground()
        , guiBackground()
        , guiLeftEye()
        , guiRightEye()
        , guiCursor()
        , cursorAnimations()
        , portraitAnimations()
        , cursorRow(DEFAULT_CURSOR_ROW)
        , cursorColumn(DEFAULT_CURSOR_COLUMN)
        , enableCursorMovement(false)
    {
        std::weak_ptr<ImageCache> imageCache = services.locateService<ImageCache>(Services::IMAGECACHE);
        std::weak_ptr<AnimationSetCache> animationCache = services.locateService<AnimationSetCache>(Services::ANIMATIONSETCACHE);

        // Load sprites from config
        // TODO: This needs to be refactored to be safer and things like that
        // TODO: Need a utility method to load sf:Sprite from JSON
        if(auto imageCachePtr = imageCache.lock()) {
            background.setTexture(*imageCachePtr->get(params[PROPERTY_BACKGROUND].asString()).get());
            foreground.setTexture(*imageCachePtr->get(params[PROPERTY_FOREGROUND].asString()).get());
            leftEye.setTexture(*imageCachePtr->get(params[PROPERTY_EYE_SPRITE].asString()).get());
            rightEye.setTexture(*imageCachePtr->get(params[PROPERTY_EYE_SPRITE].asString()).get());
        }

        guiCursor.first.reset(new gui::Icon(params[PROPERTY_CURSOR_SPRITE].asString()));

        guiCursor.first->setX(-100);
        guiCursor.first->setY(-100);

        // Load eye positions
        // There are 9 positions; 18 points total (one for left, one for right eye)
        const Json::Value &jsonEyePositions = params[PROPERTY_EYE_POSITIONS];
        if(static_cast<int>(jsonEyePositions.size()) == NUM_OF_PORTRAITS) {
            for(int i = 0; i < NUM_OF_PORTRAITS; ++i) {
                const Json::Value &jsonEyePosition = jsonEyePositions[i];
                float leftEyeX = jsonEyePosition[0u].get(PROPERTY_X, 0).asFloat();
                float leftEyeY = jsonEyePosition[0u].get(PROPERTY_Y, 0).asFloat();
                float rightEyeX = jsonEyePosition[1u].get(PROPERTY_X, 0).asFloat();
                float rightEyeY = jsonEyePosition[1u].get(PROPERTY_Y, 0).asFloat();

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
                float cursorX = jsonCursorPosition.get(PROPERTY_X, 0).asFloat();
                float cursorY = jsonCursorPosition.get(PROPERTY_Y, 0).asFloat();

                Point2D<float> position(cursorX, cursorY);

                cursorPositions.push_back(position);
            }
        }

        if(auto animations = animationCache.lock()) {
            cursorAnimations = animations->get("assets/animations/cursor.json");
            portraitAnimations = animations->get("assets/animations/stage-select.json");
        }

        buildGui();
    }

    StageSelectState::~StageSelectState() {

    }

    void StageSelectState::selectCurrentPortrait() {
        calculateCursorIndex();

        // Set eye positions
        const auto & eyePosition = config.getEyePositions().at(cursorIndex);

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

        guiCursor.first->setX(cursorPosition.getX());
        guiCursor.first->setY(cursorPosition.getY());
    }

    void StageSelectState::calculateCursorIndex() {
        cursorIndex = (cursorRow * NUM_OF_CURSOR_ROWS) + cursorColumn;
    }

    void StageSelectState::buildGui() {
        guiContainer->setSize(256, 240);
        guiContainer->setBaseColor(0x1f22AA);
        guiContainer->setOpaque(false);
        guiContainer->setVisible(true);

        guiFlashLayer->setSize(guiContainer->getWidth(), guiContainer->getHeight());
        guiFlashLayer->setBaseColor(0xffffff);
        guiFlashLayer->setOpaque(true);
        guiFlashLayer->setVisible(false);

        guiBossIntroLayer->setSize(guiContainer->getWidth(), guiContainer->getHeight());
        guiBossIntroLayer->setBaseColor(0x000000);
        guiBossIntroLayer->setOpaque(true);
        guiBossIntroLayer->setVisible(false);
        guiBossIntroLayer->setEnabled(true);

        guiBossStripe->setSize(guiBossIntroLayer->getWidth(), 96);
        guiBossStripe->setBaseColor(0x1f22AA);
        guiBossStripe->setOpaque(true);
        guiBossStripe->setVisible(true);
        // Center vertically
        guiBossStripe->setPosition(0, guiBossIntroLayer->getHeight() / 2 - guiBossStripe->getHeight() / 2);

        guiBossIntroLabel->setY(guiBossStripe->getHeight() - 16);
        guiBossIntroLabel->setCaption("TESTING THIS");
        guiBossIntroLabel->adjustSize();
        guiBossIntroLabel->setX((guiBossStripe->getWidth() / 2) - (guiBossIntroLabel->getWidth() / 2));

        guiSelectedCellLabel->setX(8);
        guiSelectedCellLabel->setY(224);
        guiSelectedCellLabel->setCaption("(" + StringUtils::toString(cursorColumn) + ", " + StringUtils::toString(cursorRow) + ")");
        guiSelectedCellLabel->adjustSize();

        guiBackground.reset(new gui::Icon(config.getBackground()));
        guiForeground.reset(new gui::Icon(config.getForeground()));

        const auto & eyeSpriteInfo = config.getEyeInfo();
        guiLeftEye.reset(new gui::Icon("assets/images/eye-stage-select.png"));
        guiRightEye.reset(new gui::Icon("assets/images/eye-stage-select.png"));

        if(cursorAnimations) {
            guiCursor.first.reset(new gui::Icon(cursorAnimations->getImageFileName()));
            guiCursor.second.reset(new gui::IconAnimator(*guiCursor.first.get()));
            guiCursor.second->setAnimation(cursorAnimations->get("default"));
        }

        if(portraitAnimations) {
            for(unsigned int i = 0; i < NUM_OF_PORTRAITS; ++i) {
                const auto & position = cursorPositions.at(i);

                AnimatedIcon portraitIcon;
                portraitIcon.first.reset(new gui::Icon(portraitAnimations->getImageFileName()));
                portraitIcon.first->setPosition(position.getX() + 8, position.getY() + 8);
                portraitIcon.second.reset(new gui::IconAnimator(*portraitIcon.first.get()));

                // TODO: Look up the correct portrait from the JSON.
                portraitIcon.second->setAnimation(portraitAnimations->get("default"));

                portraits.push_back(std::move(portraitIcon));
            }
        }

        guiContainer->add(guiBackground.get());

        for(unsigned int i = 0; i < NUM_OF_PORTRAITS; ++i) {
            // 4 is where Megaman's portrait is
            if(i != 4) {
                const auto & portrait = portraits.at(i);
                guiContainer->add(portrait.first.get());
            }
        }

        const auto & portraitInfo = config.getPortraits();
        for(unsigned int i = 0; i < NUM_OF_PORTRAITS; ++i) {
            const auto & position = cursorPositions.at(i);
            const auto & info = portraitInfo.at(i);
            const auto & cursor = guiCursor.first;

            std::unique_ptr<gcn::LabelEx> label(new gcn::LabelEx(info.label));
            label->setPosition(position.getX(), position.getY() + cursor->getHeight());
            label->adjustSize();

            guiContainer->add(label.get());
            portraitLabels.push_back(std::move(label));
        }

        guiContainer->add(guiLeftEye.get());
        guiContainer->add(guiRightEye.get());
        guiContainer->add(guiSelectedCellLabel.get());
        guiContainer->add(guiCursor.first.get());

        guiBossIntroLayer->add(guiBossStripe.get());
        guiBossStripe->add(guiBossIntroLabel.get());
        guiContainer->add(guiBossIntroLayer.get());
        guiContainer->add(guiFlashLayer.get());
        guiFlashLayer->requestMoveToTop();
    }

    void StageSelectState::handleEvent(sf::Event &event) {
        bool playSample = false;
        const float SINGLE_FRAME = 1.0f / 60.0f;

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
                    // taskQueue.push(std::make_shared<WaitTask>(0.5f));

                    // Stop the regular music, start playing the boss intro music
                    taskQueue.push(std::make_shared<FunctionTask>(0, [&](float dt) -> bool {
                        if(auto audio = audioService.lock()) {
                            audio->stopMusic();
                            audio->playMusic("Boss Selected (MM3)");
                        }

                        return true;
                    }));

                    // Flash the screen 5 times.
                    for(int numFlashes = 0; numFlashes < 4; ++numFlashes) {
                        taskQueue.push(std::make_shared<FunctionTask>(0, [&](float dt) -> bool {
                            guiFlashLayer->setVisible(true);
                            return true;
                        }));

                        taskQueue.push(std::make_shared<WaitTask>(SINGLE_FRAME * 2));

                        taskQueue.push(std::make_shared<FunctionTask>(0, [&](float dt) -> bool {
                            guiFlashLayer->setVisible(false);
                            return true;
                        }));

                        taskQueue.push(std::make_shared<WaitTask>(SINGLE_FRAME * 2));
                    }

                    // Show the boss stripe thing (where the boss does his dance)
                    taskQueue.push(std::make_shared<FunctionTask>(0, [&](float dt) -> bool {
                        guiBossIntroLayer->setVisible(true);
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
    }

    bool StageSelectState::update(float dt) {
        if(!taskQueue.empty()) {
            auto & task = taskQueue.front();
            task->update(dt);

            if(task->isComplete()) {
                taskQueue.pop();
            }
        } else {
            selectCurrentPortrait();

            guiCursor.second->update(dt);
        }

        return startGamePlay;
    }

    void StageSelectState::onEnter() {
        startGamePlay = false;
        enableCursorMovement = true;

        // Start music
        if(auto audio = audioService.lock()) {
            audio->playMusic(config.getMusicName());
        }

        // Reset cursor to default location
        cursorColumn = DEFAULT_CURSOR_COLUMN;
        cursorRow = DEFAULT_CURSOR_ROW;

        guiBossIntroLayer->setVisible(false);

        if(auto gp = gameProgress.lock()) {
            portraits.at(0).first->setVisible(!gp->bossIsDefeated(0));
            portraits.at(1).first->setVisible(!gp->bossIsDefeated(1));
            portraits.at(2).first->setVisible(!gp->bossIsDefeated(2));
            portraits.at(3).first->setVisible(!gp->bossIsDefeated(3));
            portraits.at(5).first->setVisible(!gp->bossIsDefeated(4));
            portraits.at(6).first->setVisible(!gp->bossIsDefeated(5));
            portraits.at(7).first->setVisible(!gp->bossIsDefeated(6));
            portraits.at(8).first->setVisible(!gp->bossIsDefeated(7));
        }

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

        guiBossIntroLayer->setVisible(false);

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
