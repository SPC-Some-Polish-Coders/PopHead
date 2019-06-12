#include "exitCommand.hpp"

#include "Base/gameData.hpp"

void ph::ExitCommand::execute(GameData* gameData)
{
	gameData->getRenderer().getWindow().close();
}
