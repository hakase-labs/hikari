#ifndef HIKARI_CLIENT_GUI_INPUTHELPER
#define HIKARI_CLIENT_GUI_INPUTHELPER

#include "hikari/client/game/Input.hpp"

#include <guichan/key.hpp>

#include <unordered_map>

namespace hikari {
namespace gui {

    /**
     * InputHelper is a helper class for getting hikari::Input mapped keys from
     * Guichan keys. This allows integration between Guichan key events and
     * "virtual gamepad" buttons defined by Hikari.
     */
    class InputHelper {
    private:
        static std::unordered_map<int, Input::Button> keyToButtonMapping;

        // Make constructors private so we can't instantiate this class
        InputHelper();
        InputHelper(const InputHelper & proto);

    public:
        /**
         * Gets the Button mapped to a particular key.
         *
         * @param  key the key to check the mapping for
         * @return     the Input::Button mapped to key, or BUTTON_NONE if
         *             no mapping exists.
         */
        static Input::Button getMappedButtonForKey(const gcn::Key & key);

        /**
         * Creates a mapping between a specific Key and Button.
         *
         * @param key    the Guichan key to map
         * @param button the Input::Button to map the key to
         */
        static void setMappedButtonForKey(const gcn::Key & key, Input::Button button);
    };

} // hikari::gui
} // hikari

#endif // HIKARI_CLIENT_GUI_INPUTHELPER
