#ifndef HIKARI_CLIENT_GAME_OBJECTS_PALETTEDANIMATEDSPRITE
#define HIKARI_CLIENT_GAME_OBJECTS_PALETTEDANIMATEDSPRITE

#include "hikari/client/game/objects/AnimatedSprite.hpp"

#include <memory>

namespace sf {
    class Shader;
}

namespace hikari {
    class PalettedAnimatedSprite : public AnimatedSprite {
    private:
        static std::unique_ptr<sf::Shader> pixelShader;
        
        // The index of the "shared palette", used to color multiple things (hero, powerups, etc.)
        static int sharedPaletteIndex;

        int paletteIndex;
    protected:

    public:
        static const int USE_NO_PALETTE_INDEX;
        static const int USE_SHARED_PALETTE_INDEX;
        static void setShaderFile(const std::string & file);

        PalettedAnimatedSprite();
        PalettedAnimatedSprite(const PalettedAnimatedSprite & proto);
        virtual ~PalettedAnimatedSprite();

        //
        // Inherited from hikari::Updatable
        //
        virtual void update(float dt);

        virtual void render(sf::RenderTarget &target) const;

        int getPaletteIndex() const;
        void setPaletteIndex(int index);
    };
} // hikari

#endif // HIKARI_CLIENT_GAME_OBJECTS_PALETTEDANIMATEDSPRITE