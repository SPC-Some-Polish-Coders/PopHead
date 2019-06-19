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

	void executeEcho();

	void executeExit();

	void executeTeleport();
	sf::Vector2f getTeleportPositionFromCommand() const;
	sf::Vector2f handleTeleportArgumentError() const;
	void executeCurrentPos();
	auto getPlayer() const -> Object&;

	void executeHistory();

	void executeCollisionDebug();
	void changeCollisionDebugColor();
	void changeCollisionDebugDisplayMode();
	void turnOnOrTurnOffCollisionDebug();
	
	void executeMute();
	void executeUnmute();
	void setAudioMuted(bool mute);
	void executeSetVolume();
	float getVolumeFromCommand();

	void executeLog();
	void logInto();
	void setLogTypesToLog();
	inline bool areArgumentsToLogTypesToLogInvalid();
	void setModulesToLog();
	inline bool areArgumentsToModulesToLogInvalid();

	bool commandContains(const char);
	bool commandContains(const char*);

private:
	std::string mCommand;
	GameData* mGameData;
};

}

#endif // !POPHEAD_TERMINAL_COMMANDS_COMMANDINTERPRETER_H_

