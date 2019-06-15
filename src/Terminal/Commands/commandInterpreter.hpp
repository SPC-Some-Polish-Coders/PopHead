#ifndef POPHEAD_TERMINAL_COMMANDS_COMMANDINTERPRETER_H_
#define POPHEAD_TERMINAL_COMMANDS_COMMANDINTERPRETER_H_

#include <string>
#include <SFML/Graphics.hpp>

namespace ph {

class GameData;
class Object;

class CommandInterpreter
{
public:
	void setGameData(GameData* gameData) { mGameData = gameData; }

	void handleCommand(const std::string&);
private:
	std::string getCommandWithoutArguments();
	int getArgumentPositionInCommand();

	void executeLog();

	void executeExit();

	void executeTeleport();
	sf::Vector2f getPositionFromCommand() const;
	void executeCurrentPos();
	auto getPlayer() const -> Object&;

	void executeCollisionDebug();
	void changeCollisionDebugColor();
	void changeCollisionDebugDisplayMode();
	void turnOnOrTurnOffCollisionDebug();
	
	void executeMute();
	void executeUnmute();
	void executeSetVolume();
	float getVolumeFromCommand();

	void executeSetLoggingIntoFile();
	void executeSetLoggingIntoConsole();
	void executeSetLogging();
	void executeSetLoggingLogTypes();
	void executeSetLoggingModuleNames();

	bool commandContains(const char);
	bool commandContains(const std::string&);

private:
	std::string mCommand;
	GameData* mGameData;
};

}

#endif // !POPHEAD_TERMINAL_COMMANDS_COMMANDINTERPRETER_H_

