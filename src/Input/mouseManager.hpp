#ifndef POPHEAD_INPUT_MOUSEMANAGER_H_
#define POPHEAD_INPUT_MOUSEMANAGER_H_

#include <SFML/Graphics.hpp>

namespace PopHead {
    namespace Base{ class GameData; }
namespace Input {

class MouseManager
{
public:
    auto getMousePosition() const -> sf::Vector2i;
    bool hasMouseMovedSinceLastInput() const;
    bool isMouseButtonPressed(sf::Mouse::Button) const;
    bool isMouseButtonReleased(sf::Mouse::Button) const;

    void setGameData(Base::GameData* gameData){mGameData = gameData;}

private:
    Base::GameData* mGameData;
};

}}

#endif // !POPHEAD_INPUT_MOUSEMANAGER_H_
