#include "hikari/core/game/Direction.hpp"

namespace hikari {

    const Direction Directions::None = Direction(-1);
    const Direction Directions::Up = Direction(0);
    const Direction Directions::Right = Direction(1);
    const Direction Directions::Down = Direction(2);
    const Direction Directions::Left = Direction(3);

    Directions::Directions() {

    }

    Direction Directions::opposite(const Direction& dir) {
        if(dir == Directions::Up) {
            return Directions::Down;
        } else if(dir == Directions::Right) {
            return Directions::Left;
        } else if(dir == Directions::Down) {
            return Directions::Up;
        } else if(dir == Directions::Left) {
            return Directions::Right;
        }

        return Directions::None;
    }

} // hikari