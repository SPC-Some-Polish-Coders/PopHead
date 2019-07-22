#pragma once

#include <string>
#include <SFML/Graphics.hpp>

namespace ph {

class GameData;
class DrawableGameObject;

class CommandInterpreter
{
public:
	void setGameData(GameData* const gameData) { mGameData = gameData; }

	void handleCommand(const std::string&);
private:
	std::string getCommandWithoutArguments() const;
	int getArgumentPositionInCommand() const;

	void executeEcho() const;

	void executeExit() const;

	void executeTeleport() const;
	void executeCurrentPos() const;
	auto getPlayer() const -> DrawableGameObject&;

	void executeHistory() const;
	void executeHelp() const;
	void executeClear() const;

	void executeCollisionDebug() const;
	void changeCollisionDebugColor() const;
	void changeCollisionDebugDisplayMode() const;
	void turnOnOrTurnOffCollisionDebug() const;
	
	void executeMute() const;
	void executeUnmute() const;
	void setAudioMuted(bool mute) const;
	void executeSetVolume() const;
	float getVolumeFromCommand() const;

	void executeLog() const;
	void logInto() const;
	void setLogTypesToLog() const;
	bool areArgumentsToLogTypesToLogInvalid() const;
	void setModulesToLog() const;
	bool areArgumentsToModulesToLogInvalid() const;

	void executeView() const;

	auto getVector2Argument() const -> sf::Vector2f;
	sf::Vector2f handleGetVector2ArgumentError() const;

	bool commandContains(const char) const;
	bool commandContains(const char*) const;

private:
	std::string mCommand;
	const sf::Vector2f mVector2ArgumentError = {-1, -1};
	GameData* mGameData;
};

}
