#ifndef HIKARI_CLIENT_GAME_OBJECTS_PALETTEDANIMATEDSPRITE
#define HIKARI_CLIENT_GAME_OBJECTS_PALETTEDANIMATEDSPRITE

#include "hikari/client/game/objects/AnimatedSprite.hpp"

#include <SFML/Graphics/Color.hpp>

#include <memory>
#include <vector>

namespace sf {
    class Image;
    class Shader;
    class Texture;
}

namespace hikari {
    class PalettedAnimatedSprite : public AnimatedSprite {
    private:
        static std::unique_ptr<sf::Shader> pixelShader;
        static std::unique_ptr<sf::Image> colorTableImage;
        static std::unique_ptr<sf::Texture> colorTableTexture;
        static std::vector<std::vector<sf::Color>> colorTable;

        // The index of the "shared palette", used to color multiple things (hero, powerups, etc.)
        static int sharedPaletteIndex;
        static const unsigned int colorTableWidth;
        static const unsigned int colorTableHeight;

        int paletteIndex;
        bool usePalette;
        bool useSharedPalette;

    public:
        static void setShaderFile(const std::string & file);
        static void createColorTable(const std::vector<std::vector<sf::Color>> & colors);
        static void destroySharedResources();
        static void setSharedPaletteIndex(int index);
        static int getSharedPaletteIndex();
        static const std::vector<std::vector<sf::Color>> & getColorTable();

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

        bool isUsingSharedPalette() const;
        void setUseSharedPalette(bool flag);

        bool isUsingPalette() const;
        void setUsePalette(bool flag);
    };
} // hikari

#endif // HIKARI_CLIENT_GAME_OBJECTS_PALETTEDANIMATEDSPRITE
