#include "command.hpp"

ph::Command::Command(GameData* const gameData, const std::string& command)
	:mGameData(gameData)
	,mCommand(command)
{
}
