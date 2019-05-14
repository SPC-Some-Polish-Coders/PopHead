#ifndef POPHEAD_STATES_GAMESTATE_GAMESTATE_HPP
#define POPHEAD_STATES_GAMESTATE_GAMESTATE_HPP

#include "States/state.hpp"
#include <SFML/Graphics.hpp>

namespace PopHead{
namespace States{


class GameState : public State
{
public:
    GameState(Base::GameData *const);
private:
    void loadResources();
    void makeSceneTree();
    void makeMap();
	void makeWall(); //this wall is for test of static collisions
    void makeBoat();
	void makeNpc();
	void makeNpcToBeAbleToTestDynamicCollisions();
    void makePlayer();
	void makeStaticObjectToCamera();
    void setCamera();

public:
    void input() override;
private:
	void handleCollisionDebugShortcuts();
	void switchCollisionDebugMode();
	void turnOnAndTurnOffCollisionDebugSettings();

public:
    void update(sf::Time delta) override;
private:
	void cameraShake();
	void cameraMovement(sf::Time delta) const;
	void boatMovement(sf::Time delta);

private:
	bool mShouldCameraShake = false;
	bool mIsCollisionDebugTurnOn = false;
	int mCollisionDebugMode = 1;
};


}}

#endif // !POPHEAD_STATES_GAMESTATE_GAMESTATE_HPP
