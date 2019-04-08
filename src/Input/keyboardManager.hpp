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
    bool isKeyJustPressed( sf::Keyboard::Key );

    void setGameData(Base::GameData* gameData){mGameData = gameData;}

private:
    std::map<sf::Keyboard::Key, bool> mPreviouslyPressed;
    Base::GameData* mGameData;
};

}}

#endif // !POPHEAD_INPUT_KEYBOARDMANAGER_H_
