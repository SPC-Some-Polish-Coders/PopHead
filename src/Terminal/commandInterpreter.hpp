#pragma once

#include "terminalSharedData.hpp"
#include <string>
#include <unordered_map>
#include <entt/entt.hpp>
#include <SFML/System/Vector2.hpp>

namespace ph {

class SceneManager;
class TerminalRenderer;

enum class MessageType 
{ 
	ERROR, INFO, USER, BLANK
};

class CommandInterpreter
{
public:
	void init(SceneManager*, TerminalSharedData, TerminalRenderer*);

	void update(float dt);

	void handleCommand(const std::string&);
private:
	std::string getCommandWithoutArguments() const;
	int getArgumentPositionInCommand() const;

	void executeInfoMessage();
	void executeGotoScene();
	void executePause();
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
	void executeResetGuiLiveFrequency();

#endif 

	void setAudioMuted(bool mute) const;
	float getSingleFloatArgument() const;
	auto getVector2Argument() const -> sf::Vector2f;
	sf::Vector2f handleGetVector2ArgumentError() const;

	bool commandContains(const char) const;
	bool commandContains(const char*) const;

	void executeMessage(const std::string&, const MessageType) const;

private:
	std::unordered_map<std::string, void(CommandInterpreter::*)() > mCommandsMap;
	std::string mCommand;
	TerminalSharedData mTerminalSharedData;
	TerminalRenderer* mTerminalRenderer;
	SceneManager* mSceneManager;
	const sf::Vector2f mVector2ArgumentError = {-1, -1};

#ifndef PH_DISTRIBUTION

	struct ResetGuiLive
	{
		float timeFromReset = 0.f;
		float resetFrequency = 0.2f;
		bool isActive = false;
	};
	ResetGuiLive mResetGuiLive;

#endif 
};

}
