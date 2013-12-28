#ifndef HIKARI_CLIENT_GAME_GAMEPLAYSTATE
#define HIKARI_CLIENT_GAME_GAMEPLAYSTATE

#include "hikari/core/game/GameState.hpp"
#include "hikari/core/game/map/Camera.hpp"
#include "hikari/core/game/map/Map.hpp"
#include "hikari/client/game/events/EventData.hpp"
#include "hikari/client/game/events/EventListenerDelegate.hpp"
#include "hikari/client/game/objects/EntityDeathType.hpp"

#include "hikari/client/game/GameWorld.hpp"
#include "hikari/core/game/map/RoomTransition.hpp"


#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/View.hpp>
#include <SFML/Graphics/RectangleShape.hpp>

#include <memory>

#include <queue>
#include <string>
#include <list>
#include <map>
#include <vector>
#include <utility>


// #include <oolua.h>

namespace Json {
    class Value;
}

namespace gcn {
    class Container;
    class Label;
    class ActionListener;
    class SelectionListener;
}

namespace hikari {

    namespace gui {
        class EnergyMeter;
        class EnergyGauge;
        class Panel;
        class Menu;
        class Icon;
    }

    class AudioService;
    class GameConfig;
    class GameController;
    class GameProgress;
    class GuiService;
    class ImageFont;
    class MapLoader;
    class ServiceLocator;
    class ImageCache;
    class SquirrelService;
    class RealTimeInput;
    class Door;
    class Room;
    class Map;
    class MapRenderer;
    class Hero;
    class TileMapCollisionResolver;
    class Spawner;
    class EventBus;
    class KeyboardInput;
    class WeaponTable;
    class DamageTable;
    class Particle;
    class CollectableItem;
    class Task;

    /**
     * GamePlayState is the class where actual stages are played. Most of the
     * game mechanics exist in this state. These mechanics include starting of
     * stages, handling spawning of enemies and items, and pretty much anything
     * else that the player actually "plays" in the game.
     *
     * The primary mechanics of the game are implemented as a series of 
     * substates within this class. Only one substate can be active at a time.
     * Different substates include the period before Rock teleports in (called
     * the "Ready" substate), then the period when he *is* teleporting in,
     * then the period when the player can actually play, etc.
     */
    class GamePlayState : public GameState {

    class SubState;

    private:
        std::string name;
        GameController & controller;
        std::weak_ptr<AudioService> audioService;
        std::weak_ptr<GuiService> guiService;
        std::shared_ptr<EventBus> eventBus;
        std::weak_ptr<WeaponTable> weaponTable;
        std::weak_ptr<DamageTable> damageTable;
        std::weak_ptr<GameConfig> gameConfig;
        std::weak_ptr<GameProgress> gameProgress;
        std::shared_ptr<ImageCache> imageCache;
        std::shared_ptr<RealTimeInput> userInput;
        std::shared_ptr<SquirrelService> scriptEnv;
        std::shared_ptr<TileMapCollisionResolver> collisionResolver;
        std::shared_ptr<Map> currentMap;
        std::shared_ptr<Tileset> currentTileset;
        std::shared_ptr<Room> currentRoom;
        std::shared_ptr<Hero> hero;
        std::unique_ptr<MapRenderer> mapRenderer;
        std::unique_ptr<SubState> subState;
        std::unique_ptr<SubState> nextSubState;
        std::unique_ptr<gcn::Container> guiContainer;
        std::unique_ptr<gui::EnergyGauge> guiBossEnergyGauge;
        std::unique_ptr<gui::EnergyGauge> guiHeroEnergyGauge;
        std::unique_ptr<gui::EnergyGauge> guiWeaponEnergyGauge;
        std::unique_ptr<gui::Panel> guiMenuPanel;
        std::unique_ptr<gui::Icon> guiWeaponMenuBackground;
        std::unique_ptr<gui::EnergyGauge> guiMenuLifeEnergyGauge;
        std::unique_ptr<gcn::Label> guiLivesLabel;
        std::unique_ptr<gcn::Label> guiETanksLabel;
        std::unique_ptr<gcn::Label> guiReadyLabel;
        std::unique_ptr<gui::Menu> guiWeaponMenu;
        std::unique_ptr<gcn::ActionListener> guiWeaponMenuActionListener;
        std::unique_ptr<gcn::SelectionListener> guiWeaponMenuSelectionListener;
        std::unique_ptr<KeyboardInput> keyboardInput;
        std::map< std::string, std::shared_ptr<Map> > maps;
        std::vector<std::weak_ptr<Spawner>> itemSpawners;
        std::vector<std::weak_ptr<Spawner>> deactivatedItemSpawners;
        std::vector<std::pair<EventListenerDelegate, EventType>> eventHandlerDelegates;
        std::list<std::pair<int, std::string>> bonusChancesTable;
        std::queue<std::shared_ptr<Task>> taskQueue;
        GameWorld world;
        Camera camera;
        sf::View view;
        sf::RectangleShape spawnerMarker;
        sf::RectangleShape transitionMarker;
        sf::RectangleShape leftBar;
        bool drawInfamousBlackBar;
        bool canViewMenu;
        bool isViewingMenu;
        bool hasReachedMidpoint;
        bool hasReachedBossCorridor;
        bool isHeroAlive;
        bool gotoNextState;
        bool isRestoringEnergy;

        //
        // Resource Management
        //
        void loadAllMaps(const std::weak_ptr<MapLoader> &mapLoader, const Json::Value &params);

        //
        // Gameplay Mechanics
        //
        
        /**
         * Immediately changes from one substate to another. This causes the 
         * current substate's "exit()" method to be called before calling the
         * "enter()" method of the new substate. If either the current or the
         * new substate is null it will be ignored (no methods will be called
         * on it).
         *
         * Note: be careful when using this between substates. Changing states
         * will cause the "old" state to be deleted -- so don't call this from
         * the middle of a method in a substate. Doing so will lead to undefined
         * behavior and most likely a SEGFAULT.
         * 
         * @param newSubState the new substate to enter in to
         */
        void changeSubState(std::unique_ptr<SubState> && newSubState);

        /**
         * Enqueues a substate to be entered in to. This is safer than using
         * changeSubState directly. If you want to change from one substate to
         * another from *within* a substate then use this method to enqeue the
         * next state.
         * 
         * @param newSubState the new substate to enter into when safe
         */
        void requestSubStateChange(std::unique_ptr<SubState> && newSubState);

        /**
         * Changes which room is the current room. The current room is used for
         * variaous things -- most importantly it is used for collision 
         * detection and also spawning enemies and items. Only one room can be
         * "current" at a time.
         *
         * Changing rooms causes cleanup to take place in the old room before
         * setting the new room as current. The current room should not be
         * tamplered with -- it should only be changed with this method.
         * 
         * @param newCurrentRoom pointer to new room (which will become current)
         */
        void changeCurrentRoom(const std::shared_ptr<Room>& newCurrentRoom);

        /**
         * Creates links to all spawners in a given Room without taking
         * ownership of them. This causes spawners to attach event listeners and
         * also puts them in an "asleep" state.
         *
         * @see checkSpawners
         */
        void linkSpawners(const std::shared_ptr<Room> & room);

        /**
         * Checks all linked spawners if they should be active or inactive. This
         * checks if they are on screen or not. Only spawners that are active
         * are checked. Active spawners on screen will be woken up, active awake
         * spawners that or off screen will be put to bed.
         *
         * @see linkSpawners
         */
        void checkSpawners(float dt);

        /**
         * Cleans up any stale objects between room transitions and level restarts.
         */
        void cleanStaleObjects();

        /**
         * Builds a table of probabilites for bonus drop items.
         */
        void populateBonusChancesTable();

        /**
         * Spawns a "random" bonus item and returns a pointer to the instance.
         * If the random drop is "nothing" then a null pointer is returned.
         */
        std::shared_ptr<CollectableItem> spawnBonusItem(int bonusTableIndex = 0);

        /**
         * Spawns a death explosion -- like when the hero or a boss is defeated.
         * @param type the EntityDeathType of explosion
         * @param position where the particles should radiate from
         */
        void spawnDeathExplosion(EntityDeathType::Type type, const Vector2<float> & position);

        /**
         * Starts the current stage from the beginning. This is like playing the
         * level for the first time. This causes the state of the stage to be
         * reset.
         *
         * @see startRound
         */
        void startStage();

        /**
         * Starts a round of the current stage from the closest starting point.
         *
         * Starting a new round does not reset the state of the stage (items 
         * collected won't reappear, etc.) It's like starting the level after
         * you just died.
         *
         * @see startStage
         */
        void startRound();

        /**
         * Checks if the player is colliding with a transition region.
         */
        void checkCollisionWithTransition();

        /**
         * Updates doors of the current room if it has any.
         */
        void updateDoors(float dt);

        //
        // Rendering
        //
        void renderMap(sf::RenderTarget &target) const;
        void renderHero(sf::RenderTarget &target) const;
        void renderEntities(sf::RenderTarget &target) const;
        void renderHud(sf::RenderTarget &target) const;

        //
        // Event handlers
        //
        void bindEventHandlers();
        void handleEntityDamageEvent(EventDataPtr evt);
        void handleEntityDeathEvent(EventDataPtr evt);
        void handleWeaponFireEvent(EventDataPtr evt);
        void handleEntityStateChangeEvent(EventDataPtr evt);
        void handleDoorEvent(EventDataPtr evt);

        //
        // GUI
        //
        void buildGui();

        /**
         * GamePlayState::SubState encapsulates a part of gameplay that operates
         * independently from others. Some examples of this would be:
         *  - Starting a stage ("READY")
         *  - Teleporting megaman in from the top of the screen
         *  - Actually plasying a stage
         *  - Transitioning from one room to another (automatic camera movement)
         *  - Ending a stage (success or death)
         *  - Pausing / menu screen
         */
        class SubState {
        protected:
            GamePlayState & gamePlayState;
            SubState(GamePlayState & gamePlayState);
        public:
            enum StateChangeAction {
                CONTINUE,
                NEXT
            };
            virtual ~SubState() { };
            virtual void enter() = 0;
            virtual void exit() = 0;
            virtual StateChangeAction update(const float & dt) = 0;
            virtual void render(sf::RenderTarget &target) = 0;
        };

        /**
         * Shows the "READY" animation and then starts the level.
         */
        class ReadySubState : public SubState {
        private:
            bool renderReadyText;
            bool renderFadeOverlay;
            float timer;
            sf::RectangleShape fadeOverlay;

        public:
            ReadySubState(GamePlayState & gamePlayState);
            virtual ~ReadySubState();
            virtual void enter();
            virtual void exit();
            virtual StateChangeAction update(const float & dt);
            virtual void render(sf::RenderTarget &target);
        };

        /**
         * Teleporting sequence, in our out of the screen.
         */
        class TeleportSubState : public SubState {
        private:
            Vector2<float> startingPoint;
            Vector2<float> targetPoint;
        public:
            TeleportSubState(GamePlayState & gamePlayState);
            virtual ~TeleportSubState();
            virtual void enter();
            virtual void exit();
            virtual StateChangeAction update(const float & dt);
            virtual void render(sf::RenderTarget &target);
        };

        /**
         * Actual game play, player-controlled movements, etc.
         */
        class PlayingSubState : public SubState {
        private:
            float postDeathTimer; // A timer that counts after you die
        public:
            PlayingSubState(GamePlayState & gamePlayState);
            virtual ~PlayingSubState();
            virtual void enter();
            virtual void exit();
            virtual StateChangeAction update(const float & dt);
            virtual void render(sf::RenderTarget &target);
        };

        /**
         * Sequence where hero transitions between rooms.
         */
        class TransitionSubState : public SubState {
        private:
            static const float transitionSpeedX;
            static const float transitionSpeedY;
            static const float heroTranslationSpeedX;
            static const float heroTranslationSpeedY;
            static const float doorDelay;

            float transitionEndX;
            float transitionEndY;
            int transitionFrames;
            bool transitionFinished;
            float doorDelayIn;
            float doorDelayOut;

            RoomTransition transition;
            Rectangle2D<int> nextRoomCullRegion;
            std::shared_ptr<Room> nextRoom;
            std::shared_ptr<Door> entranceDoor;
            std::shared_ptr<Door> exitDoor;

            std::shared_ptr<Room> findNextRoom() const;
        public:
            TransitionSubState(GamePlayState & gamePlayState, RoomTransition transition);
            virtual ~TransitionSubState();
            virtual void enter();
            virtual void exit();
            virtual StateChangeAction update(const float & dt);
            virtual void render(sf::RenderTarget &target);
        };

    public:
        GamePlayState(const std::string &name, GameController & controller, const Json::Value &params, const std::weak_ptr<GameConfig> & gameConfig, ServiceLocator &services);
        virtual ~GamePlayState();

        virtual void handleEvent(sf::Event &event);
        virtual void render(sf::RenderTarget &target);
        virtual bool update(const float &dt);
        virtual void onEnter();
        virtual void onExit();
        virtual const std::string &getName() const;
    };

} // hikari

#endif // HIKARI_CLIENT_GAME_GAMEPLAYSTATE
