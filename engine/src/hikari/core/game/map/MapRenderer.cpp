#include "hikari/core/game/map/MapRenderer.hpp"
#include "hikari/core/game/map/Room.hpp"
#include "hikari/core/game/map/Tileset.hpp"
#include <SFML/Graphics.hpp>
#include <algorithm>
#include <cmath>

#ifdef _DEBUG
#include <iostream>
#endif

namespace hikari {
    
    const int MapRenderer::TILE_OVERDRAW = 1;

    MapRenderer::MapRenderer(const RoomPtr &room, const TileDataPtr &tileData)
        : room(room)
        , tileData(tileData)
        , tileSprite()
        , visibleScreenArea()
        , visibleTileArea() {
        if(tileData) {
            tileSprite.setTexture(*(tileData->getTexture()));    
        }
    }

    void MapRenderer::setRoom(const RoomPtr &room) {
        if(this->room != room) {
            this->room = room;
            cullTiles();
        }
    }

    RoomPtr MapRenderer::getRoom() const {
        return room;
    }

    void MapRenderer::setTileData(const TileDataPtr &tileData) {
        this->tileData = tileData;
        tileSprite.setTexture(*(tileData->getTexture()));
        cullTiles();
    }

    TileDataPtr MapRenderer::getTileData() const {
        return tileData;
    }

    void MapRenderer::render(sf::RenderTarget &target) {
        int tileIndex = Room::NO_TILE;
        int tileAttributes = TileAttribute::NO_ATTRIBUTES;

        for(int y = visibleTileArea.getY(); y < visibleTileArea.getHeight(); ++y) {
            for(int x = visibleTileArea.getX(); x < visibleTileArea.getWidth(); ++x) {
                tileIndex = room->getTileAt(x, y);
                tileAttributes = room->getAttributeAt(x, y);

                if(tileIndex == Room::NO_TILE) {
                    continue;
                }

                applyTileToSprite(tileIndex, x, y);
                applyTileAttributes(tileAttributes);

                // TODO: Not sure about this... it may make the jittering MORE obvious...?
                const auto & unQuantizedPosition = tileSprite.getPosition();
                tileSprite.setPosition(std::ceil(unQuantizedPosition.x), std::ceil(unQuantizedPosition.y));

                target.draw(tileSprite);
            }
        }

        sf::RectangleShape ladderRect;
        ladderRect.setFillColor(sf::Color(128, 128, 0, 96));
        ladderRect.setOutlineColor(sf::Color(255, 255, 255, 128));
        ladderRect.setOutlineThickness(1.0f);

        std::for_each(std::begin(room->getLadders()), std::end(room->getLadders()), [this, &ladderRect, &target](const BoundingBox<float> & ladder) {
            ladderRect.setPosition(sf::Vector2f(ladder.getLeft(), ladder.getTop()));
            ladderRect.setSize(sf::Vector2f(ladder.getWidth(), ladder.getHeight()));

            target.draw(ladderRect);
        });
    }

    inline void MapRenderer::cullTiles() {
        if(tileData != nullptr && room != nullptr) {
            const int tileSize = tileData->getTileSize();

            int x = visibleScreenArea.getX() / tileSize;
            int width = visibleScreenArea.getWidth() / tileSize;
            int y = visibleScreenArea.getY() / tileSize;
            int height = visibleScreenArea.getHeight() / tileSize;

            // Find minimum and maximum tile rendering ranges
            x = std::max(x, room->getX());
            width = std::min(x + width + TILE_OVERDRAW, room->getWidth() + room->getX());
            y = std::max(y, room->getY());
            height = std::min(y + height + TILE_OVERDRAW, room->getHeight() + room->getY());

            visibleTileArea.setX(x);
            visibleTileArea.setWidth(width);
            visibleTileArea.setY(y);
            visibleTileArea.setHeight(height);
        }
    }

    void MapRenderer::setCullRegion(const Rectangle2D<int> &cullRegion) {
        visibleScreenArea = cullRegion;
        cullTiles();
    }

    inline void MapRenderer::applyTileToSprite(const int &tileIndex, const int &x, const int &y) {
        const sf::IntRect& rect = tileData->getTileRect(tileIndex);

        tileSprite.setTextureRect(rect);
        tileSprite.setPosition(
            static_cast<float>(x * tileData->getTileSize()), 
            static_cast<float>(y * tileData->getTileSize())
        );
    }

    inline void MapRenderer::applyTileAttributes(const int &attributes) {
        //tileSprite.FlipX(false);
        //tileSprite.FlipY(false);
        tileSprite.setRotation(0.0f);
        tileSprite.setOrigin(0.0f, 0.0f);

        if((attributes & TileAttribute::FLIP_HORIZONTAL) == TileAttribute::FLIP_HORIZONTAL) {
            //tileSprite.FlipX(true);
        }

        if((attributes & TileAttribute::FLIP_VERTICAL) == TileAttribute::FLIP_VERTICAL) {
            //tileSprite.FlipY(true);
        }

        if((attributes & TileAttribute::ROTATE_BY_90) == TileAttribute::ROTATE_BY_90) {
            float tileRotationOffset = static_cast<float>(tileData->getTileSize() * 0.5f);
            tileSprite.setOrigin(tileRotationOffset, tileRotationOffset);
            tileSprite.setRotation(90.0f);
            tileSprite.move(tileRotationOffset, tileRotationOffset);
        }
    }

} // hikari