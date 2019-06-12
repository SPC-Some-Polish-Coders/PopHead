#ifndef POPHEAD_TERMINAL_COMMANDS_COMMANDINTERPRETER_H_
#define POPHEAD_TERMINAL_COMMANDS_COMMANDINTERPRETER_H_

#include <string>

namespace ph {

class GameData;

class CommandInterpreter
{
public:
	void setGameData(GameData* gameData) { mGameData = gameData; }

	void handleCommand(const std::string&);
private:
	bool commandHasAnArgument();
	bool commandContains(const char);

	void handleCommandWithOneArgument();
	std::string getCommandWithoutArguments();
	int getArgumentPositionInCommand();
	void executeLog();
	
	void handleCommandWithoutArguments();

private:
	std::string mCommand;
	GameData* mGameData;
};

}

#endif // !POPHEAD_TERMINAL_COMMANDS_COMMANDINTERPRETER_H_

