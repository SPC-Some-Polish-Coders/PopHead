#ifndef POPHEAD_INPUT_KEYBOARDMANAGER_H_
#define POPHEAD_INPUT_KEYBOARDMANAGER_H_

#include <SFML/Window.hpp>

namespace PopHead {
namespace Input {

class KeyboardManager
{
public:
    bool isKeyPressed( sf::Keyboard::Key ) const;
    bool isKeyJustPressed( sf::Keyboard::Key ) const;

private:
    bool mPressed;
    sf::Event::KeyEvent::Code mKeyCode;
};

}}

#endif // !POPHEAD_INPUT_KEYBOARDMANAGER_H_
