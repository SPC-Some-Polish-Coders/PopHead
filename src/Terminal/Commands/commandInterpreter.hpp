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
	std::string getCommandWithoutArguments();
	int getArgumentPositionInCommand();

	void executeLog();

	void executeChangeCollisionDebugColors();
	void executeChangeCollisionDebugDisplay();
	void executeSwitchCollisionDebugMode();
	
	void executeMute();
	void executeUnmute();
	void executeSetVolume();
	float getVolumeFromCommand();

	void executeSetLoggingIntoFile();
	void executeSetLoggingIntoConsole();
	void executeSetLogging();
	void executeSetLoggingLogTypes();
	void executeSetLoggingModuleNames();

	void executeExit();

	bool commandContains(const char);
	bool commandContains(const char*);

private:
	std::string mCommand;
	GameData* mGameData;
};

}

#endif // !POPHEAD_TERMINAL_COMMANDS_COMMANDINTERPRETER_H_

