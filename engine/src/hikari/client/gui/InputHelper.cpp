#include "hikari/client/gui/InputHelper.hpp"

namespace hikari {
namespace gui {

    std::unordered_map<int, Input::Button> InputHelper::keyToButtonMapping = std::unordered_map<int, Input::Button>();
    
    InputHelper::InputHelper() {

    }

    InputHelper::InputHelper(const InputHelper & proto) {

    }

    Input::Button InputHelper::getMappedButtonForKey(const gcn::Key & key) {
        auto mappedButton = keyToButtonMapping.find(key.getValue());
        
        if(mappedButton != keyToButtonMapping.end()) {
            return mappedButton->second;
        }

        return Input::BUTTON_NONE;
    }

    void InputHelper::setMappedButtonForKey(const gcn::Key & key, Input::Button button) {
        keyToButtonMapping[key.getValue()] = button;
    }

} // hikari::gui
} // hikari
