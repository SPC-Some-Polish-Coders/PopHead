#ifndef POPHEAD_INPUT_KEYBOARDMANAGER_H_
#define POPHEAD_INPUT_KEYBOARDMANAGER_H_

#include <SFML/Window.hpp>
#include <map>

namespace PopHead {
    namespace Base{ class GameData; }
namespace Input {

class KeyboardManager
{
public:
    bool isKeyPressed( sf::Keyboard::Key ) const;
    bool isKeyJustPressed( sf::Keyboard::Key ) const;
    bool isKeyJustReleased( sf::Keyboard::Key ) const;

    void setGameData(Base::GameData* gameData){mGameData = gameData;}

private:
    bool isKeyJust(sf::Event::EventType type, sf::Keyboard::Key key) const;

private:
    Base::GameData* mGameData;
};

}}

#endif // !POPHEAD_INPUT_KEYBOARDMANAGER_H_
