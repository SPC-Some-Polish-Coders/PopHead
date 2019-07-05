#pragma once

#include <string>
#include <SFML/Graphics.hpp>

namespace ph {

class GameData;
class Object;
class Map;

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
	bool areArgumentsToLogTypesToLogInvalid();
	void setModulesToLog();
	bool areArgumentsToModulesToLogInvalid();

	void executeView();
	auto getMap() const -> Map&;

	auto getVector2Argument() const -> sf::Vector2f;
	sf::Vector2f handleGetVector2ArgumentError() const;

	bool commandContains(const char);
	bool commandContains(const char*);

private:
	std::string mCommand;
	GameData* mGameData;

};

}
