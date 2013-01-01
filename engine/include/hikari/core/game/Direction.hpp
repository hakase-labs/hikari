#ifndef HIKARI_CORE_GAME_DIRECTION
#define HIKARI_CORE_GAME_DIRECTION

namespace hikari {

    typedef int Direction;

    class Directions {
    public:
        static const Direction None;
        static const Direction Up;
        static const Direction Right;
        static const Direction Down; 
        static const Direction Left;  

        static Direction opposite(const Direction& dir);
    private:
        Directions();
    };

} // hikari

#endif // HIKARI_CORE_GAME_DIRECTION