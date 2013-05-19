#ifndef GCN_SDLINPUT_HPP
#define GCN_SDLINPUT_HPP

#include <queue>

#include "guichan/input.hpp"
#include "guichan/keyinput.hpp"
#include "guichan/mouseinput.hpp"
#include "guichan/platform.hpp"

#include <SFML/Window/Event.hpp>
#include <SFML/System/Clock.hpp>

namespace sf
{
    class RenderTarget;
}

namespace gcn
{
    class Key;

    /**
     * SFML implementation of Input.
     */
    class GCN_EXTENSION_DECLSPEC SFMLInput : public Input
    {
    public:

        /**
         * Constructor.
         */
        SFMLInput();

        /**
         * Pushes an SFML event. It should be called at least once per frame to
         * update input with user input. The RenderTarget should be provided in order
         * to calculate coordinates of mouse events accurately.
         *
         * @param event an event from SFML.
         */
        virtual void pushInput(const sf::Event& event, const sf::RenderTarget& target);

        /**
         * Polls all input. It exists for input driver compatibility.
         */
        virtual void _pollInput() { }


        // Inherited from Input

        virtual bool isKeyQueueEmpty();

        virtual KeyInput dequeueKeyInput();

        virtual bool isMouseQueueEmpty();

        virtual MouseInput dequeueMouseInput();

    protected:
        /**
         * Converts a mouse button from SFML to a Guichan mouse button
         * representation.
         *
         * @param button an SFML mouse button.
         * @return a Guichan mouse button.
         */
        int convertMouseButton(sf::Mouse::Button button);
                
        /**
         * Converts an SFML event to a Guichan key value.
         *
         * @param event The SFML event to convert.
         * @return A Guichan key value. -1 if no conversion took place.
         * @see Key
         */
        int convertSFMLEventToGuichanKeyValue(sf::Event event);

        std::queue<KeyInput> mKeyInputQueue;
        std::queue<MouseInput> mMouseInputQueue;

        bool mMouseDown;
        bool mMouseInWindow;

        sf::Clock mClock;
    };
}

#endif // end GCN_SFMLINPUT_HPP
