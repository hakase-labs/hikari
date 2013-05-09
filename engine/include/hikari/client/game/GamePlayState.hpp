#ifndef HIKARI_CLIENT_GAME_GAMEPLAYSTATE
#define HIKARI_CLIENT_GAME_GAMEPLAYSTATE

#include "hikari/core/game/GameState.hpp"
#include "hikari/core/game/map/Camera.hpp"
#include "hikari/core/game/map/Map.hpp"
#include "hikari/client/game/events/EventData.hpp"
#include "hikari/client/game/events/EventListenerDelegate.hpp"

#include "hikari/client/game/GameWorld.hpp"
#include "hikari/core/game/map/RoomTransition.hpp"

#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/View.hpp>
#include <SFML/Graphics/RectangleShape.hpp>

#include <memory>

#include <string>
#include <map>
#include <vector>
#include <utility>


// #include <oolua.h>

namespace Json {
    class Value;
}

namespace hikari {

    namespace gui {
        class EnergyMeter;
    }

    class AudioService;
    class GameProgress;
    class ImageFont;
    class MapLoader;
    class ServiceLocator;
    class ImageCache;
    class SquirrelService;
    class RealTimeInput;
    class Room;
    class Map;
    class MapRenderer;
    class Hero;
    class TileMapCollisionResolver;
    class Spawner;
    class EventManager;

    class GamePlayState : public GameState {

    class SubState;

    private:
        std::string name;
        std::weak_ptr<AudioService> audioService;
        std::shared_ptr<EventManager> eventManager;
        std::shared_ptr<GameProgress> gameProgress;
        std::shared_ptr<ImageFont> guiFont;
        std::shared_ptr<ImageCache> imageCache;
        std::shared_ptr<RealTimeInput> userInput;
        std::shared_ptr<SquirrelService> scriptEnv;
        std::shared_ptr<TileMapCollisionResolver> collisionResolver;
        std::shared_ptr<Map> currentMap;
        std::shared_ptr<Room> currentRoom;
        std::shared_ptr<Hero> hero;
        std::shared_ptr<gui::EnergyMeter> hudBossEnergyMeter;
        std::shared_ptr<gui::EnergyMeter> hudHeroEnergyMeter;
        std::shared_ptr<gui::EnergyMeter> hudCurrentWeaponMeter;
        std::unique_ptr<MapRenderer> mapRenderer;
        std::unique_ptr<SubState> subState;
        std::map< std::string, std::shared_ptr<Map> > maps;
        std::vector<std::weak_ptr<Spawner>> itemSpawners;
        std::vector<std::weak_ptr<Spawner>> deactivatedItemSpawners;
        std::vector<std::pair<EventListenerDelegate, EventType>> eventHandlerDelegates;
        GameWorld world;
        Camera camera;
        sf::View view;
        sf::RectangleShape spawnerMarker;
        sf::RectangleShape transitionMarker;
        sf::RectangleShape leftBar;
        sf::RectangleShape menuPlaceholder;
        bool drawBossEnergyMeter;
        bool drawHeroEnergyMeter;
        bool drawWeaponEnergyMeter;
        bool drawInfamousBlackBar;
        bool isViewingMenu;
        bool hasReachedMidpoint;
        bool hasReachedBossCorridor;
        bool isHeroAlive;

        //
        // Resource Management
        //
        void loadAllMaps(const std::weak_ptr<MapLoader> &mapLoader, const Json::Value &params);

        //
        // Gameplay Mechanics
        //
        void changeSubState(std::unique_ptr<SubState> && newSubState);
        void changeCurrentRoom(const std::shared_ptr<Room>& newCurrentRoom);

        /**
            Creates links to all spawners in a given Room without taking
            ownership of them.
        */
        void linkSpawners(const std::shared_ptr<Room> & room);

        /**
            Checks all linked spawners if they should be active or inactive.
        */
        void checkSpawners();

        /**
            Starts the current stage from the beginning.
        */
        void startStage();

        /**
            Restarts the current stage from the closest restart point.
        */
        void restartStage();

        /**
            Handles actions that take place after a stage has been completed.
        */
        void endStage();

        void playerBirth();
        void playerDeath();

        /**
            Checks if the player is colliding with a transition region.
            If it is then some kind of flag needs to be set to begin a transition.
        */
        void checkCollisionWithTransition();

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
        void handleEntityDeathEvent(EventDataPtr evt);
        void handleWeaponFireEvent(EventDataPtr evt);
        void handleEntityStateChangeEvent(EventDataPtr evt);

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
            virtual ~SubState() { };
            virtual void enter() = 0;
            virtual void exit() = 0;
            virtual void update(const float & dt) = 0;
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
            virtual void update(const float & dt);
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
            virtual void update(const float & dt);
            virtual void render(sf::RenderTarget &target);
        };

        /**
         * Actual game play, player-controlled movements, etc.
         */
        class PlayingSubState : public SubState {
        public:
            PlayingSubState(GamePlayState & gamePlayState);
            virtual ~PlayingSubState();
            virtual void enter();
            virtual void exit();
            virtual void update(const float & dt);
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

            float transitionEndX;
            float transitionEndY;
            int transitionFrames;
            bool transitionFinished;

            RoomTransition transition;
            Rectangle2D<int> nextRoomCullRegion;
            std::shared_ptr<Room> nextRoom;

            std::shared_ptr<Room> findNextRoom() const;
        public:
            TransitionSubState(GamePlayState & gamePlayState, RoomTransition transition);
            virtual ~TransitionSubState();
            virtual void enter();
            virtual void exit();
            virtual void update(const float & dt);
            virtual void render(sf::RenderTarget &target);
        };

    public:
        GamePlayState(const std::string &name, const Json::Value &params, ServiceLocator &services);
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
