#include "guichan/sfml/sfmlinput.hpp"

#include <SFML/Graphics/RenderTarget.hpp>

#include "guichan/exception.hpp"

namespace gcn
{
    SFMLInput::SFMLInput()
    {
        mMouseInWindow = true;
        mMouseDown = false;
    }

    bool SFMLInput::isKeyQueueEmpty()
    {
        return mKeyInputQueue.empty();
    }

    KeyInput SFMLInput::dequeueKeyInput()
    {
        KeyInput keyInput;

        if (mKeyInputQueue.empty())
        {
            throw GCN_EXCEPTION("The queue is empty.");
        }

        keyInput = mKeyInputQueue.front();
        mKeyInputQueue.pop();

        return keyInput;
    }

    bool SFMLInput::isMouseQueueEmpty()
    {
        return mMouseInputQueue.empty();
    }

    MouseInput SFMLInput::dequeueMouseInput()
    {
        MouseInput mouseInput;

        if (mMouseInputQueue.empty())
        {
            throw GCN_EXCEPTION("The queue is empty.");
        }

        mouseInput = mMouseInputQueue.front();
        mMouseInputQueue.pop();

        return mouseInput;
    }

    void SFMLInput::pushInput(const sf::Event& event, const sf::RenderTarget& target)
    {
        KeyInput keyInput;
        MouseInput mouseInput;

        switch (event.type)
        {
            case sf::Event::KeyPressed:
            {
                int value = convertSFMLEventToGuichanKeyValue(event);

                if (value == -1)
                {
                    value = (int)event.key.code; // This is actually wrong; but what can you do?
                } 
               
                keyInput.setKey(Key(value));
                keyInput.setType(KeyInput::Pressed);
                keyInput.setShiftPressed(event.key.shift);
                keyInput.setControlPressed(event.key.control);
                keyInput.setAltPressed(event.key.alt);
                keyInput.setMetaPressed(event.key.system);
                keyInput.setNumericPad(event.key.code >= sf::Keyboard::Numpad0
                                       && event.key.code <= sf::Keyboard::Numpad9);

                mKeyInputQueue.push(keyInput);
                break;
            }

            case sf::Event::KeyReleased:
            {
                int value = convertSFMLEventToGuichanKeyValue(event);

                if (value == -1)
                {
                    value = (int)event.key.code; // This is actually wrong; but what can you do?
                } 
               
                keyInput.setKey(Key(value));
                keyInput.setType(KeyInput::Released);
                keyInput.setShiftPressed(event.key.shift);
                keyInput.setControlPressed(event.key.control);
                keyInput.setAltPressed(event.key.alt);
                keyInput.setMetaPressed(event.key.system);
                keyInput.setNumericPad(event.key.code >= sf::Keyboard::Numpad0
                                       && event.key.code <= sf::Keyboard::Numpad9);

                mKeyInputQueue.push(keyInput);
                break;
            }

            case sf::Event::MouseButtonPressed:
            {
                sf::Vector2i mouseCoords(event.mouseButton.x, event.mouseButton.y);
                sf::Vector2f normalizedCoords = target.mapPixelToCoords(mouseCoords);

                mMouseDown = true;
                mouseInput.setX(static_cast<int>(normalizedCoords.x));
                mouseInput.setY(static_cast<int>(normalizedCoords.y));
                mouseInput.setButton(convertMouseButton(event.mouseButton.button));
                mouseInput.setType(MouseInput::Pressed);
                mouseInput.setTimeStamp(mClock.getElapsedTime().asMilliseconds());

                mMouseInputQueue.push(mouseInput);
                break;
            }
            case sf::Event::MouseButtonReleased:
            {
                sf::Vector2i mouseCoords(event.mouseButton.x, event.mouseButton.y);
                sf::Vector2f normalizedCoords = target.mapPixelToCoords(mouseCoords);

                mMouseDown = false;
                mouseInput.setX(static_cast<int>(normalizedCoords.x));
                mouseInput.setY(static_cast<int>(normalizedCoords.y));
                mouseInput.setButton(convertMouseButton(event.mouseButton.button));
                mouseInput.setType(MouseInput::Released);
                mouseInput.setTimeStamp(mClock.getElapsedTime().asMilliseconds());

                mMouseInputQueue.push(mouseInput);
                break;
            }
            case sf::Event::MouseMoved:
            {
                sf::Vector2i mouseCoords(event.mouseMove.x, event.mouseMove.y);
                sf::Vector2f normalizedCoords = target.mapPixelToCoords(mouseCoords);

                mouseInput.setX(static_cast<int>(normalizedCoords.x));
                mouseInput.setY(static_cast<int>(normalizedCoords.y));
                mouseInput.setButton(MouseInput::Empty);
                mouseInput.setType(MouseInput::Moved);
                mouseInput.setTimeStamp(mClock.getElapsedTime().asMilliseconds());

                mMouseInputQueue.push(mouseInput);
                break;
            }
            case sf::Event::MouseWheelMoved:
            {
                sf::Vector2i mouseCoords(event.mouseWheel.x, event.mouseWheel.y);
                sf::Vector2f normalizedCoords = target.mapPixelToCoords(mouseCoords);

                mMouseDown = true;
                mouseInput.setX(static_cast<int>(normalizedCoords.x));
                mouseInput.setY(static_cast<int>(normalizedCoords.y));
                mouseInput.setButton(MouseInput::Empty);

                if (event.mouseWheel.delta > 0)
                {
                    mouseInput.setType(MouseInput::WheelMovedUp);
                }
                else if (event.mouseWheel.delta < 0)
                {
                    mouseInput.setType(MouseInput::WheelMovedDown);
                }

                mouseInput.setTimeStamp(mClock.getElapsedTime().asMilliseconds());

                mMouseInputQueue.push(mouseInput);
                break;
            }
            case sf::Event::LostFocus:
                /*
                 * This occurs when the mouse leaves the window and the Guichan
                 * application loses its mousefocus.
                 */
                mMouseInWindow = false;

                if (!mMouseDown)
                {
                    mouseInput.setX(-1);
                    mouseInput.setY(-1);
                    mouseInput.setButton(MouseInput::Empty);
                    mouseInput.setType(MouseInput::Moved);

                    mMouseInputQueue.push(mouseInput);
                }
                break;

            case sf::Event::GainedFocus:
                mMouseInWindow = true;
                break;

            default:
                break;
        }
    }

    int SFMLInput::convertMouseButton(sf::Mouse::Button button)
    {
        switch (button)
        {
        case sf::Mouse::Left:
              return MouseInput::Left;
              break;
        case sf::Mouse::Right:
              return MouseInput::Right;
              break;
        case sf::Mouse::Middle:
              return MouseInput::Middle;
              break;
          default:
              // We have an unknown mouse type which is ignored.
              return button;
        }
    }

    int SFMLInput::convertSFMLEventToGuichanKeyValue(sf::Event event)
    {
        int value = -1;

        switch(event.key.code)
        {
            case sf::Keyboard::Unknown:
                value = -1;
                break;
            case sf::Keyboard::A:
                value = 'A';
                break;
            case sf::Keyboard::B:
                value = 'B';
                break;
            case sf::Keyboard::C:
                value = 'C';
                break;
            case sf::Keyboard::D:
                value = 'D';
                break;
            case sf::Keyboard::E:
                value = 'E';
                break;
            case sf::Keyboard::F:
                value = 'F';
                break;
            case sf::Keyboard::G:
                value = 'G';
                break;
            case sf::Keyboard::H:
                value = 'H';
                break;
            case sf::Keyboard::I:
                value = 'I';
                break;
            case sf::Keyboard::J:
                value = 'J';
                break;
            case sf::Keyboard::K:
                value = 'K';
                break;
            case sf::Keyboard::L:
                value = 'L';
                break;
            case sf::Keyboard::M:
                value = 'M';
                break;
            case sf::Keyboard::N:
                value = 'N';
                break;
            case sf::Keyboard::O:
                value = 'O';
                break;
            case sf::Keyboard::P:
                value = 'P';
                break;
            case sf::Keyboard::Q:
                value = 'Q';
                break;
            case sf::Keyboard::R:
                value = 'R';
                break;
            case sf::Keyboard::S:
                value = 'S';
                break;
            case sf::Keyboard::T:
                value = 'T';
                break;
            case sf::Keyboard::U:
                value = 'U';
                break;
            case sf::Keyboard::V:
                value = 'V';
                break;
            case sf::Keyboard::W:
                value = 'W';
                break;
            case sf::Keyboard::X:
                value = 'X';
                break;
            case sf::Keyboard::Y:
                value = 'Y';
                break;
            case sf::Keyboard::Z:
                value = 'Z';
                break;
            case sf::Keyboard::Num0:
                value = '0';
                break;
            case sf::Keyboard::Num1:
                value = '1';
                break;
            case sf::Keyboard::Num2:
                value = '2';
                break;
            case sf::Keyboard::Num3:
                value = '3';
                break;
            case sf::Keyboard::Num4:
                value = '4';
                break;
            case sf::Keyboard::Num5:
                value = '5';
                break;
            case sf::Keyboard::Num6:
                value = '6';
                break;
            case sf::Keyboard::Num7:
                value = '7';
                break;
            case sf::Keyboard::Num8:
                value = '8';
                break;
            case sf::Keyboard::Num9:
                value = '9';
                break;
            case sf::Keyboard::Escape:
                value = Key::Escape;
                break;
            case sf::Keyboard::LControl:
                value = Key::LeftControl;
                break;
            case sf::Keyboard::LShift:
                value = Key::LeftShift;
                break;
            case sf::Keyboard::LAlt:
                value = Key::LeftAlt;
                break;
            case sf::Keyboard::LSystem:
                value = Key::LeftMeta;
                break;
            case sf::Keyboard::RControl:
                value = Key::RightControl;
                break;
            case sf::Keyboard::RShift:
                value = Key::RightShift;
                break;
            case sf::Keyboard::RAlt:
                value = Key::RightAlt;
                break;
            case sf::Keyboard::RSystem:
                value = Key::RightMeta;
                break;
            case sf::Keyboard::Menu:
                value = -1;
                break;
            case sf::Keyboard::LBracket:
                value = '[';
                break;
            case sf::Keyboard::RBracket:
                value = ']';
                break;
            case sf::Keyboard::SemiColon:
                value = ';';
                break;
            case sf::Keyboard::Comma:
                value = ',';
                break;
            case sf::Keyboard::Period:
                value = '.';
                break;
            case sf::Keyboard::Quote:
                value = '\'';
                break;
            case sf::Keyboard::Slash:
                value = '/';
                break;
            case sf::Keyboard::BackSlash:
                value = '\\';
                break;
            case sf::Keyboard::Tilde:
                value = '~';
                break;
            case sf::Keyboard::Equal:
                value = '=';
                break;
            case sf::Keyboard::Dash:
                value = '-';
                break;
            case sf::Keyboard::Space:
                value = Key::Space;
                break;
            case sf::Keyboard::Return:
                value = Key::Enter;
                break;
            case sf::Keyboard::BackSpace:
                value = Key::Backspace;
                break;
            case sf::Keyboard::Tab:
                value = Key::Tab;
                break;
            case sf::Keyboard::PageUp:
                value = Key::PageUp;
                break;
            case sf::Keyboard::PageDown:
                value = Key::PageDown;
                break;
            case sf::Keyboard::End:
                value = Key::End;
                break;
            case sf::Keyboard::Home:
                value = Key::Home;
                break;
            case sf::Keyboard::Insert:
                value = Key::Insert;
                break;
            case sf::Keyboard::Delete:
                value = Key::Delete;
                break;
            case sf::Keyboard::Add:
                value = -1;
                break;
            case sf::Keyboard::Subtract:
                value = -1;
                break;
            case sf::Keyboard::Multiply:
                value = -1;
                break;
            case sf::Keyboard::Divide:
                value = -1;
                break;
            case sf::Keyboard::Left:
                value = Key::Left;
                break;
            case sf::Keyboard::Right:
                value = Key::Right;
                break;
            case sf::Keyboard::Up:
                value = Key::Up;
                break;
            case sf::Keyboard::Down:
                value = Key::Down;
                break;
            case sf::Keyboard::Numpad0:
                value = -1;
                break;
            case sf::Keyboard::Numpad1:
                value = -1;
                break;
            case sf::Keyboard::Numpad2:
                value = -1;
                break;
            case sf::Keyboard::Numpad3:
                value = -1;
                break;
            case sf::Keyboard::Numpad4:
                value = -1;
                break;
            case sf::Keyboard::Numpad5:
                value = -1;
                break;
            case sf::Keyboard::Numpad6:
                value = -1;
                break;
            case sf::Keyboard::Numpad7:
                value = -1;
                break;
            case sf::Keyboard::Numpad8:
                value = -1;
                break;
            case sf::Keyboard::Numpad9:
                value = -1;
                break;
            case sf::Keyboard::F1:
                value = Key::F1;
                break;
            case sf::Keyboard::F2:
                value = Key::F2;
                break;
            case sf::Keyboard::F3:
                value = Key::F3;
                break;
            case sf::Keyboard::F4:
                value = Key::F4;
                break;
            case sf::Keyboard::F5:
                value = Key::F5;
                break;
            case sf::Keyboard::F6:
                value = Key::F6;
                break;
            case sf::Keyboard::F7:
                value = Key::F7;
                break;
            case sf::Keyboard::F8:
                value = Key::F8;
                break;
            case sf::Keyboard::F9:
                value = Key::F9;
                break;
            case sf::Keyboard::F10:
                value = Key::F10;
                break;
            case sf::Keyboard::F11:
                value = Key::F11;
                break;
            case sf::Keyboard::F12:
                value = Key::F12;
                break;
            case sf::Keyboard::F13:
                value = Key::F13;
                break;
            case sf::Keyboard::F14:
                value = Key::F14;
                break;
            case sf::Keyboard::F15:
                value = Key::F15;
                break;
            case sf::Keyboard::Pause:
                value = Key::Pause;
                break;
            default:
                break;
        }

        return value;
    }
}
