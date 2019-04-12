#define INPUT_getMousePosition mGameData->getInput().getMouse().getMousePosition
#define INPUT_hasMouseJustMoved mGameData->getInput().getMouse().hasMouseJustMoved
#define INPUT_isMouseButtonPressed mGameData->getInput().getMouse().isMouseButtonPressed
#define INPUT_isMouseButtonJustPressed mGameData->getInput().getMouse().isMouseButtonJustPressed
#define INPUT_isMouseButtonJustReleased mGameData->getInput().getMouse().isMouseButtonJustReleased


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
    bool hasMouseJustMoved() const;
    bool isMouseButtonPressed(sf::Mouse::Button) const;
    bool isMouseButtonJustPressed(sf::Mouse::Button) const;
    bool isMouseButtonJustReleased(sf::Mouse::Button) const;

    void setGameData(Base::GameData* gameData){mGameData = gameData;}

private:
    Base::GameData* mGameData;
};


}}

#endif // !POPHEAD_INPUT_MOUSEMANAGER_H_
