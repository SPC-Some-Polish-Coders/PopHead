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

	void update(float dt);

	void handleCommand(const std::string&);
private:
	std::string getCommandWithoutArguments() const;
	int getArgumentPositionInCommand() const;

	void executeInfoMessage();
	void executeEcho();
	void executeExit();
	void executeGotoScene();
	void executeResetGui();
	void executeTeleport();
	void executeCurrentPos();
	void executeMove();
	void executeGive();
	void executeHistory();
	void executeHelp();
	void executeClear();
	void executeCollisionDebug();
	void executeVelocityChangingAreaDebug();
	void executePushingAreaDebug();
	void executeMute();
	void executeUnmute();
	void executeSetVolume();
	void executeView();
	void executeLight();
	void executeFontDebug();
	void executeNoFocusUpdate();

#ifndef PH_DISTRIBUTION

	void executeResetGuiLive();

#endif 

	void setAudioMuted(bool mute) const;
	float getVolumeFromCommand() const;
	auto getVector2Argument() const -> sf::Vector2f;
	sf::Vector2f handleGetVector2ArgumentError() const;

	bool commandContains(const char) const;
	bool commandContains(const char*) const;

	void executeMessage(const std::string&, const MessageType) const;

private:
	std::unordered_map<std::string, void(CommandInterpreter::*)() > mCommandsMap;
	std::string mCommand;
	const sf::Vector2f mVector2ArgumentError = {-1, -1};
	GameData* mGameData;
	entt::registry* mSceneRegistry;

#ifndef PH_DISTRIBUTION

	struct ResetGuiLive
	{
		float timeFromReset = 0.f;
		bool isActive = false;
	};
	ResetGuiLive mResetGuiLive;

#endif 
};

}
