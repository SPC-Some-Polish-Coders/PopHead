#pragma once

#include <SFML/Graphics.hpp>

namespace ph {

class GameData;

class MouseManager
{
public:
    auto getMousePosition() const -> sf::Vector2i;
    bool hasMouseJustMoved() const;
    bool isMouseButtonJustPressed(sf::Mouse::Button) const;
    bool isMouseButtonJustReleased(sf::Mouse::Button) const;

    void setGameData(GameData* gameData){mGameData = gameData;}

private:
    GameData* mGameData;
};


}
