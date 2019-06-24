#pragma once

#include "gameData.hpp"

namespace ph {

void loadFonts(GameData* gameData)
{
	gameData->getFonts().load("fonts/consola.ttf");
	gameData->getFonts().load("fonts/joystixMonospace.ttf");
}

}
