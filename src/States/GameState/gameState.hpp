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
    void makePlayer();
    void makeBoat();
	void makeNPC();
    void makeMap();
	void makeWall(); //this wall is for test of static collisions
    void setCamera();
	void makeStaticObjectToCamera();

public:
    void input() override;

public:
    void update(sf::Time delta) override;
private:
	void cameraShake();
	void cameraMovement(sf::Time delta) const;
	void boatMovement(sf::Time delta);

private:
	bool shouldCameraShake = false;
};


}}

#endif // !POPHEAD_STATES_GAMESTATE_GAMESTATE_HPP
