#ifndef POPHEAD_TERMINAL_COMMANDS_COMMAND_H_
#define POPHEAD_TERMINAL_COMMANDS_COMMAND_H_

#include <string>

namespace ph {

class GameData;

class Command
{
public:
	Command(GameData* const, const std::string& command);

private:
	std::string mCommand;
	GameData* mGameData;
};

}

#endif // !POPHEAD_TERMINAL_COMMANDS_COMMAND_H_

