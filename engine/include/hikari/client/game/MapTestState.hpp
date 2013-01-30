#ifndef HIKARI_CLIENT_GAME_MAPTESTSTATE
#define HIKARI_CLIENT_GAME_MAPTESTSTATE

#include "hikari/core/game/GameState.hpp"
#include "hikari/core/game/map/Camera.hpp"
#include "hikari/core/game/map/Map.hpp"
#include "hikari/core/game/map/Room.hpp"
#include "hikari/core/game/map/MapLoader.hpp"
#include "hikari/core/game/map/MapRenderer.hpp"
#include "hikari/core/game/map/Tileset.hpp"
#include "hikari/core/game/map/TilesetLoader.hpp"
#include "hikari/core/game/map/MapLoader.hpp"
#include "hikari/core/gui/ImageFont.hpp"
#include "hikari/core/math/RetroVector.hpp"
#include "hikari/client/game/objects/ItemFactory.hpp"
#include "hikari/core/game/AnimationLoader.hpp"
#include "hikari/core/game/AnimationSet.hpp"
#include "hikari/core/game/Animation.hpp"
#include "hikari/core/game/SpriteAnimator.hpp"
#include "hikari/client/game/GameWorld.hpp"
#include "hikari/core/util/Timer.hpp"
#include <SFML/Graphics.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <SFML/Graphics/Sprite.hpp>

#include <SFML/Window/Keyboard.hpp>
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/Graphics/View.hpp>
#include <string>
#include <vector>

#include <squirrel.h>
#include <sqstdmath.h>
#include <sqrat.h>

namespace sf {
    class RenderWindow;
}

namespace hikari {

    class ServiceLocator;
    class SquirrelService;
    class AnimationSetCache;
    class TileMapCollisionResolver;
    class CollectableItem;
    class RealTimeInput;
    class Movable;
    class Hero;
    class Enemy;

    class MapTestState : public GameState {
    private:
        std::shared_ptr<ItemFactory> itemFactory;
        std::shared_ptr<SquirrelService> squirrel;
        std::shared_ptr<AnimationSetCache> animationSetCache;
        std::string name;
        std::shared_ptr<MapLoader> mapLoader;
        TileDataPtr tiles;
        
        std::shared_ptr<Map> map;
        RoomPtr currentRoom;
        RoomPtr nextRoom;
        MapRenderer renderer;
        sf::View view;
        Camera camera;
        std::shared_ptr<hikari::ImageFont> font;
        std::shared_ptr<hikari::RealTimeInput> input;

        std::shared_ptr<Hero> hero;
        std::shared_ptr<Enemy> enemy;

        std::vector<std::shared_ptr<Enemy>> enemies;

        sf::Texture spriteImage;
        sf::Sprite sprite;
        std::shared_ptr<AnimationSet> animations;
        std::shared_ptr<Animation> idleAnimation;
        std::shared_ptr<Animation> runningAnimation;
        std::shared_ptr<Animation> redRunningAnimation;
        SpriteAnimator animationPlayer;

        sf::Vector2f velocity;

        hikari::RetroVector retroGravity;
        hikari::RetroVector retroPositionY;
        hikari::RetroVector retroVelocityY;
        hikari::RetroVector retroVelocityX;
        hikari::RetroVector retroJumpVelocity;

        std::shared_ptr<hikari::CollectableItem> item;

        std::shared_ptr<TileMapCollisionResolver> collisionResolver;
        std::shared_ptr<Movable> movable;
        
        sf::RectangleShape logicalCursor;
        sf::RectangleShape renderedCursor;
        sf::RectangleShape spawnerMarker;
        sf::RectangleShape transitionMarker;

        sf::RectangleShape cameraViewportOutline;

        Vector2<float> currentHeroPosition;
        Vector2<float> previousHeroPosition;

        GameWorld world;

        sf::RenderWindow * renderWindow;

        float playFieldZoom;

        bool grounded;
        bool falling;
        int jumpIterations;
        float jumpStartY;
        float jumpEndY;

        bool collidingWithTransition;
        bool cameraFollowingPlayer;

        // Transition-related variables
        bool transitioning;
        static const float transitionSpeedX;
        static const float transitionSpeedY;
        static const float heroTranslationSpeedX;
        static const float heroTranslationSpeedY;
        float transitionEndX;
        float transitionEndY;
        int transitionFrames;
        hikari::RoomTransition::Direction transitionDirection;

        void transitionBegin();
        void transitionUpdate(float dt);
        void transitionEnd();

        Vector2<float> screenCoordToWorldCoord(const float & x, const float & y);

        void loadMap(const std::string &tileFile);

        void initializeCamera();

        void setupHero();

        void setupEnemy();
        std::shared_ptr<Enemy> spawnEnemy(const std::string& type);

        void setupItem();
    public:
        MapTestState(
            const std::string &name, 
            const std::shared_ptr<hikari::MapLoader> &mapLoader,
            const std::string &mapFile, 
            const std::string &tileFile, 
            const std::shared_ptr<hikari::ImageCache> &imageCache, 
            const std::shared_ptr<hikari::ImageFont> &font,
            ServiceLocator &services);
        virtual ~MapTestState();

        virtual void handleEvent(sf::Event &event);
        virtual void render(sf::RenderTarget &target);
        virtual bool update(const float &dt);
        virtual void onEnter();
        virtual void onExit();
        virtual const std::string &getName() const;

        void setRenderWindow(sf::RenderWindow * window);
    };

} // hikari

#endif