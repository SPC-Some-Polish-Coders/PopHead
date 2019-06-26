#pragma once

#include "States/state.hpp"
#include <SFML/Graphics.hpp>
#include "States/sceneParser.hpp"

namespace ph {

class GameState : public State
{
public:
	GameState(GameData* const);
private:
	void playMusic();
public:
	void input() override;
private:
	void handleCameraShakeShortcut();
	void windowMinimalizeAndMaximalizeShortcut();
	void shotgunShot();

public:
	void update(sf::Time delta) override;
private:
	void cameraShake();
	void cameraMovement(sf::Time delta) const;
	void updateListenerPosition();

private:
	bool mShouldCameraShake = false;
	bool mIsCollisionDebugTurnOn = false;
	int mCollisionDebugMode = 1;
};

}
