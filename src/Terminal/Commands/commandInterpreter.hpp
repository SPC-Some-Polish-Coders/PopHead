#pragma once

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
	void executeHelp();
	void executeClear();

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
