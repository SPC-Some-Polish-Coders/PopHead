#include "exitCommand.hpp"

#include "Base/gameData.hpp"

void PopHead::Terminal::ExitCommand::execute(Base::GameData* gameData)
{
	gameData->getRenderer().getWindow().close();
}
