#ifndef POPHEAD_INPUT_KEYBOARDMANAGER_H_
#define POPHEAD_INPUT_KEYBOARDMANAGER_H_

#include <SFML/Window.hpp>
#include <map>

namespace PopHead {
namespace Input {

class KeyboardManager
{
public:
    bool isKeyPressed( sf::Keyboard::Key );
    bool isKeyJustPressed( sf::Keyboard::Key );

private:
    std::map<sf::Keyboard::Key, bool> mPreviouslyPressed;
};

}}

#endif // !POPHEAD_INPUT_KEYBOARDMANAGER_H_
