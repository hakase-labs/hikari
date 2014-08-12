#ifndef HIKARI_CLIENT_GAME_RENDERABLE
#define HIKARI_CLIENT_GAME_RENDERABLE

namespace sf {
    class RenderTarget;
}

namespace hikari {

    /**
     * Interface for all things that can be rendered to the screen.
     */
    class Renderable {
    public:
        /**
         * Virtual destructor; standard stuff.
         */
        virtual ~Renderable() {
            // no-op
        }

        /**
         * Renders an object to the render target.
         * @param target the render target
         */
        virtual void render(sf::RenderTarget & target) = 0;

        /**
         * Sets the z-index of this Renderable. The z-index affects rendering order.
         * @param index a new value for this object's z-index
         */
        virtual void setZIndex(int index) = 0;

        /**
         * Gets the z-index of this Renderable.
         * @return the z-index
         */
        virtual int getZIndex() const = 0;
    };

} // hikari

#endif // HIKARI_CLIENT_GAME_RENDERABLE
