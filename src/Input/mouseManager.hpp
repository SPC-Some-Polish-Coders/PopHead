#ifndef POPHEAD_INPUT_MOUSEMANAGER_H_
#define POPHEAD_INPUT_MOUSEMANAGER_H_

#include <SFML/Graphics.hpp>

namespace PopHead {
    namespace Base{ class GameData; }
namespace Input {

class MouseManager
{
public:
    sf::Event::MouseButtonEvent getMouseButtonEvent() const;
    sf::Vector2f getMousePosition() const;

    void setGameData(Base::GameData* gameData){mGameData = gameData;}

private:
    Base::GameData* mGameData;
};

}}

#endif // !POPHEAD_INPUT_MOUSEMANAGER_H_
