#ifndef POPHEAD_STATES_GAMESTATE_GAMESTATE_HPP
#define POPHEAD_STATES_GAMESTATE_GAMESTATE_HPP

#include "States/state.hpp"
#include <SFML/Graphics.hpp>

namespace PopHead {
namespace States {


class GameState : public State
{
public:
	GameState(Base::GameData* const);
private:
	void loadResources();
	void makeSceneTree();
	void makeMap();
	void makeWall();
	void makeBoat();
	void makeNpc();
	void makeNpcToBeAbleToTestDynamicCollisions();
	void makePlayer();
	void makeZombie();
	void makeBox();
	void makeBall();
	void makeStaticObjectToCamera();
	void setCamera();
	void playMusic();

public:
	void input() override;
private:
	void handleCollisionDebugShortcuts();
	void switchCollisionDebugMode();
	void turnOnAndTurnOffCollisionDebugSettings();
	void windowMinimalizeAndMaximalizeShortcut();
	void audioMuteShortcut();
	bool isAudioMuteShortcutPressed();
	void shotgunShot();

public:
	void update(sf::Time delta) override;
private:
	void cameraShake();
	void cameraMovement(sf::Time delta) const;
	void boatMovement(sf::Time delta);
	void updateListenerPosition();

private:
	bool mShouldCameraShake = false;
	bool mIsCollisionDebugTurnOn = false;
	int mCollisionDebugMode = 1;
};


}}

#endif // !POPHEAD_STATES_GAMESTATE_GAMESTATE_HPP
