#pragma once

#include "gameData.hpp"

namespace ph {

void loadFonts(GameData* gameData)
{
	gameData->getFonts().load("fonts/consolab.ttf");
}

}