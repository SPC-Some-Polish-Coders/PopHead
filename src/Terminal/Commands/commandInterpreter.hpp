#pragma once

#include <string>
#include <SFML/Graphics.hpp>
#include <unordered_map>
#include <entt/entt.hpp>

namespace ph {

class GameData;

enum class MessageType 
{ 
	ERROR, INFO, USER, BLANK
};

class CommandInterpreter
{
public:
	void setGameData(GameData* const gameData) { mGameData = gameData; }
	void setSceneRegistry(entt::registry* registry) { mSceneRegistry = registry; }
	void init();

	void handleCommand(const std::string&);
private:
	std::string getCommandWithoutArguments() const;
	int getArgumentPositionInCommand() const;

	void executeInfoMessage() const;
	void executeEcho() const;

	void executeExit() const;

	void executeGotoScene() const;
	void executeTeleport() const;
	void executeCurrentPos() const;
	void executeMove() const;
	void executeGive() const;

	void executeHistory() const;
	void executeHelp() const;
	void executeClear() const;

	void executeCollisionDebug() const;
	void executeVelocityChangingAreaDebug() const;
	void executePushingAreaDebug() const;
	
	void executeMute() const;
	void executeUnmute() const;
	void setAudioMuted(bool mute) const;
	void executeSetVolume() const;
	float getVolumeFromCommand() const;

	void executeView() const;

	void executeLight() const;

	auto getVector2Argument() const -> sf::Vector2f;
	sf::Vector2f handleGetVector2ArgumentError() const;

	bool commandContains(const char) const;
	bool commandContains(const char*) const;

	void executeMessage(const std::string&, const MessageType) const;

private:
	std::unordered_map<std::string, void(CommandInterpreter::*)() const> mCommandsMap;
	std::string mCommand;
	const sf::Vector2f mVector2ArgumentError = {-1, -1};
	GameData* mGameData;
	entt::registry* mSceneRegistry;
};

}
